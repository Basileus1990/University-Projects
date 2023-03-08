if(window.sessionStorage) {
    const formName = 'form'
    function init() {
        let form = {
            name: '',
            surname: '',
            email: '',
            age: '',
            sex: null,
            message_type: '',
            message: ''
        };
        sessionStorage.setItem(formName, JSON.stringify(form));
    }

    function fillForm() {
        let form = JSON.parse(sessionStorage.getItem(formName));
        document.querySelector('#name').value = form.name;
        document.querySelector('#surname').value = form.surname;
        document.querySelector('#email').value = form.email;
        document.querySelector('#age').value = form.age;
        document.querySelector('#message-type').value = form['message-type'];
        document.querySelector('#message').value = form.message;
        if(form.sex) {
            document.querySelector(`#sex-${form.sex}`).checked = true;
        }
    }
    if(!sessionStorage.getItem(formName)) {
        init();
    }
    fillForm();

    function addToSessionStorage(e) {
        let form = JSON.parse(sessionStorage.getItem(formName));
        form[e.target.name] = e.target.value;
        sessionStorage.setItem(formName, JSON.stringify(form));
    }

    for(input of document.querySelectorAll('.input-box input')) {
        input.addEventListener('input', addToSessionStorage);
    }
    document.querySelector('.input-box select').addEventListener('input', addToSessionStorage);
    document.querySelector('.input-box textarea').addEventListener('input', addToSessionStorage);
}