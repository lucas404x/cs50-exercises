function validate() {
    const email = document.querySelector('#email');
    const subject = document.querySelector('#subject');
    const content = document.querySelector('#content');

    changeBorder(email);
    changeBorder(subject);
    changeBorder(content);
    
}

function changeBorder(field) {
    if (field.value.length == 0) {
        field.style.border = "1px solid red";
    } else {
        field.style.border = "";
    }
}