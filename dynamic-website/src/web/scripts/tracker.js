if(window.localStorage) {
    let noJsBanner = document.querySelector('.no-js-banner');
    noJsBanner.style.display = 'none';

    let goalsContainer = document.querySelector('.goals');
    let goalCreatorButton = document.querySelector('#goal-creator');

    function buildGoal(goalName, numberOfDays) {
        let li = document.createElement('li');
        li.id = goalName;
        

        let closeButton = document.createElement('button')
        closeButton.classList = 'close-button';
        closeButton.textContent = 'X';
        closeButton.addEventListener('click', () => {
            localStorage.removeItem('goalName');
            let goalsNames = localStorage.getItem('goalsNames').split(',');
            goalsNames.splice(goalsNames.indexOf(goalName), 1);
            localStorage.setItem('goalsNames', goalsNames);
            localStorage.removeItem(goalName);
            li.parentElement.removeChild(li);
        })
        li.appendChild(closeButton)

        let header = document.createElement('h3')
        header.textContent = goalName;
        li.appendChild(header)

        let days = document.createElement('div');
        let daysLabel = document.createElement('label');
        let daysSpan = document.createElement('span');
        days.classList = 'days';
        daysLabel.textContent = 'Obecna ilość dni';
        daysSpan.textContent = numberOfDays;
        days.appendChild(daysLabel);
        days.appendChild(daysSpan);
        li.appendChild(days);

        let buttons = document.createElement('div');
        let addDay = document.createElement('button');
        let resetDay = document.createElement('button');
        buttons.classList = 'goal-buttons';
        addDay.setAttribute('type', 'button');
        addDay.textContent = 'Dodaj dzień';
        resetDay.setAttribute('type', 'button');
        resetDay.textContent = 'Zresetuj licznik';

        addDay.addEventListener('click', () => {
            let newValue = parseInt(daysSpan.textContent) + 1;
            daysSpan.textContent = newValue;
            localStorage.setItem(goalName, newValue);
        });

        resetDay.addEventListener('click', () => {
            daysSpan.textContent = 0;
            localStorage.setItem(goalName, 0);
        });

        buttons.appendChild(addDay);
        buttons.appendChild(resetDay);
        li.appendChild(buttons);

        goalsContainer.appendChild(li);
    }

    function buildGoaldFromStorage() { 
        if(localStorage.getItem('goalsNames')) {
            for(const goal of localStorage.getItem('goalsNames').split(',')) {
                if(goal != 'null') {
                    buildGoal(goal, localStorage.getItem(goal));
                }
            }
        }
    }

    buildGoaldFromStorage();

    $(function() {
        function addNewGoal() {
            let userInput = document.querySelector('#new-goal-name');
            let newGoalName = userInput.value;
            if (newGoalName === '') {
                userInput.setCustomValidity('Twój cel musi mieć nazwę!');
                userInput.reportValidity();
                return
            }
            else if (localStorage.getItem(newGoalName) != null) {
                userInput.setCustomValidity('Dany cel już znajdue się w twojej liście!');
                userInput.reportValidity();
                return
            }
            else if(newGoalName.indexOf(',') != -1) {
                userInput.setCustomValidity('Zakazane jest użycie znaku: \",\"');
                userInput.reportValidity();
                return
            }
            else {
                for (element of newGoalName.split(' ')) {
                    if (element.length > 20) {
                        userInput.setCustomValidity('Najdłuższy wyraz może mieć tylko 20 znaków!');
                        userInput.reportValidity();
                        return
                    }
                }
            }
        
            let currentNames = localStorage.getItem('goalsNames');
            if(localStorage.getItem('goalsNames') == null || localStorage.getItem('goalsNames') == '') {
                localStorage.setItem('goalsNames', newGoalName)
            }
            else {
                localStorage.setItem('goalsNames', `${currentNames},${newGoalName}`);
            }
            localStorage.setItem(newGoalName, 0);
            buildGoal(newGoalName, 0);
        }

        let dialog = $('#goal-adder-dialog').dialog( {
            autoOpen: false,
            height: 200,
            width: 300,
            buttons: {
                'Dodaj cel': () => {
                    addNewGoal();
                    $('#new-goal-name').val('');
                },
                'Anuluj': () => {
                    dialog.dialog('close');
                }
            },
            close: () => {
                $('#new-goal-name').val('');
            }
        });

        $('#goal-adder-button').click(() => {
            dialog.dialog('open');
        })
    });

}