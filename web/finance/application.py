import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    data = db.execute('\
            SELECT name, symbol, SUM(shares) AS shares, price, SUM(price) AS total \
            FROM stocks WHERE user_id = :user_id GROUP BY symbol HAVING SUM(shares) != 0',
                      user_id=session["user_id"])

    data = [value for value in data if not some_value_is_null(value)]

    cash = db.execute("SELECT cash FROM users WHERE id = :id",
                      id=session["user_id"])[0]["cash"]
    data.append({
        'name': '',
        'symbol': "CASH",
        'shares': '',
        'price': '',
        'total': usd(cash)
    })

    return render_template("index.html", data=data)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")

    symbol = request.form.get("symbol")
    shares = request.form.get("shares")

    # checking if the symbol/shares is empty
    if values_is_empty(symbol, shares):
        return apology("The symbol and/or shares field is empty.")

    # checking if the symbol is valid
    quote = lookup(symbol)
    if quote == None:
        return apology("The symbol does not exist.")

    # checking if the user's cash is enough
    user_id = session["user_id"]
    cash = db.execute('SELECT cash FROM users WHERE id = :id',
                      id=user_id)[0]['cash']
    final_cash = float(cash) - float(quote["price"]) * int(shares)

    if final_cash < 0:
        return apology("You don't have cash enough.")

    # update cash on database
    db.execute("UPDATE users SET cash = :cash WHERE id = :id",
               cash=final_cash, id=user_id)

    db.execute("INSERT INTO stocks(user_id, name, symbol, shares, price, transacted) \
        VALUES(:user_id, :name, :symbol, :shares, :price, date('now'))", user_id=user_id, name=quote["name"],
               symbol=symbol, shares=shares, price=quote["price"])

    return redirect("/")


@app.route("/history")
@login_required
def history():
    data = db.execute("SELECT symbol, shares, price, transacted FROM stocks WHERE user_id = :user_id",
                      user_id=session["user_id"])

    return render_template("history.html", data=data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    if request.method == "GET":
        return render_template("add.html")

    cash = request.form.get("cash")
    password = request.form.get("password")
    user_password = db.execute("SELECT hash from users WHERE id = :id",
                               id=session["user_id"])[0]["hash"]

    if password == "" or cash == "":
        return apology("The fields are empty.")
    if not check_password_hash(user_password, password):
        return apology("The passwords don't match.")

    db.execute("UPDATE users SET cash = cash + :cash WHERE id = :id",
               cash=cash, id=session["user_id"])

    return redirect("/")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "GET":
        return render_template("quote.html", quote=None)

    symbol = request.form.get("symbol").strip()

    # checking if the user forgot the symbol
    if values_is_empty(symbol):
        return apology("You forgot the symbol.")

    # checking if the symbol exists
    _quote = lookup(symbol)
    if _quote == None:
        return apology("The symbol does not exist.")

    return render_template("quote.html", quote=_quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")

    username = request.form.get("username")
    password = request.form.get("password")
    password_again = request.form.get("password_again")

    # checking if the user forgot something
    if values_is_empty(username, password, password_again):
        return apology("You forgot the username/password.")

    usernames = db.execute("SELECT username from users")
    # checking if the username exists in database
    if username in usernames:
        return apology("The username already exists.")

    # checking if the passwords are equal
    if password != password_again:
        return apology("The passwords don't match.")

    db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash)",
               username=username, hash=generate_password_hash(password))

    # set the session
    session["user_id"] = db.execute(
        "SELECT id FROM users WHERE username = :username", username=username)[0]["id"]

    return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        symbols = db.execute(
            'SELECT symbol FROM stocks WHERE user_id = :user_id \
            GROUP BY symbol HAVING SUM(shares) != 0', user_id=session["user_id"])

        print(symbols)
        symbols = [symbol["symbol"] for symbol in symbols]
        print(symbols)

        return render_template("sell.html", symbols=symbols)

    symbol = request.form.get('symbols')
    shares = int(request.form.get('shares'))
    user_id = session["user_id"]

    if symbol == None or values_is_empty(shares):
        return apology("You forgot of symbol or/and shares field.")

    quote = lookup(symbol)
    if quote == None:
        return apology("The symbol does not exist.")

    shares_in_db = db.execute('SELECT SUM(shares) AS shares FROM stocks WHERE user_id = :user_id AND symbol = :symbol',
                              user_id=user_id, symbol=symbol)
    print(shares_in_db)
    if len(shares_in_db) == 0:
        return apology("You don't have stocks that symbol.")
    elif shares_in_db[0]["shares"] < 0:
        return apology("You don't have stocks that symbol.")

    shares_in_db = int(shares_in_db[0]["shares"])
    if shares_in_db < shares:
        return apology("You don't have shares enough.")

    db.execute("INSERT INTO stocks(user_id, name, symbol, shares, price, transacted) \
        VALUES(:user_id, :name, :symbol, :shares, :price, date('now'))", user_id=user_id, name=quote["name"],
               symbol=symbol, shares=(-shares), price=quote["price"])

    db.execute("UPDATE users SET cash = (cash + :new_cash) WHERE id = :id",
               new_cash=(shares * quote["price"]), id=user_id)

    return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


def values_is_empty(*values) -> bool:
    for value in values:
        if value == '':
            return True
    return False


def some_value_is_null(data: dict) -> bool:
    return None in data.items()
