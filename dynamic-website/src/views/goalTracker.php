<!doctype html>
<html>

<head lang="pl-PL">
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <meta name="description" content="Strona o samorozwoju. O tym jak zacząć i z elementami ułatwiającymi rozpoczęcie tej pięknej przygody">
    <title>Samorozwój</title>
    <link rel="icon" type="image/x-icon" href="images/favicon.ico">

    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.13.2/themes/base/jquery-ui.min.css" integrity="sha512-ELV+xyi8IhEApPS/pSj66+Jiw+sOT1Mqkzlh8ExXihe4zfqbWkxPRi8wptXIO9g73FSlhmquFlUOuMSoXz5IRw==" crossorigin="anonymous" referrerpolicy="no-referrer" />
    <link rel="stylesheet" href="styles/header.css">
    <link rel="stylesheet" href="styles/nav.css">
    <link rel="stylesheet" href="styles/style.css">
    <link rel="stylesheet" href="styles/tracker.css">

    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.6.0/jquery.min.js" integrity="sha512-894YE6QWD5I59HgZOGReFYm4dnWc1Qt5NtvYSaNcOP+u1T9qYdvdihz0PPSiiqn/+/3e7Jo4EaG7TubfWGUrMQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.13.2/jquery-ui.min.js" integrity="sha512-57oZ/vW8ANMjR/KQ6Be9v/+/h6bq9/l3f0Oc7vn6qMqyhvPd1cvKBRWWpzu0QoneImqr2SkmO4MSqU+RpHom3Q==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    <script src="scripts/tracker.js" defer></script>
</head>

<body>
    <div class="website-body">
        <header id="myHeader">
            <h1>Samorozwój<br>Droga do lepszego życia</h1>
        </header>

        <nav>
            <label class="menu-button" for="menu-button-checkbox">
                <svg height='40' width='40'>
                    <line x1='0' y1='10' x2='40' y2='10' style='stroke:rgb(255, 255, 255);stroke-width:5' />
                    <line x1='0' y1='20' x2='40' y2='20' style='stroke:rgb(255, 255, 255);stroke-width:5' />
                    <line x1='0' y1='30' x2='40' y2='30' style='stroke:rgb(255, 255, 255);stroke-width:5' />
                </svg>
            </label>
            <input id="menu-button-checkbox" type="checkbox" name="menu">
            <ul class="nav-container">
                <li class="nav-button"><a href="/">Strona główna</a></li>
                <li class="nav-button active"><a href="#myHeader">Kalendarzyk</a></li>
                <li class="nav-button dropdown" tabindex="0">
                    Więcej
                    <ul class="dropdown-items">
                        <li class="nav-button"><a href="kontakt">Kontakt</a></li>
                        <li class="nav-button"><a href="galeria">Galeria</a></li>
                        <li class="nav-button"><a href="ulubione-zdjecia">Ulubione zdjęcia</a></li>
                        <li class="nav-button"><a href="wyszukiwarka">Wyszukiwarka</a></li>
                    </ul>
                </li>
            </ul>
        </nav>
        <?= !include 'account-info.php'; ?>
        <div class="website-content">
            <h2>Miejsce do śledzenia twoich postępów w postanowieniach</h2>
            <p class="important-paragraph">Znajdziesz tutaj nasz licznik dni, który pozwoli ci ciągle śledzić jak długo się trzymasz swojego postanowienia. Pozwoli to na zwiększenie szany na utrzymanie się nowego celu. Aby dodać nowy cel prosimy nacisnąć przycisk "<strong>+</strong>"</p>

            <h2 class="no-js-banner">Do wyświetlenia licznika dni wymagany jest włączony JavaScript i twoja przeglądarka musi wspierać localSstorage. Prosimy, aby włączyć JavaScript i użyć przeglądarki obsługującej localStorage w celu skorzystania z naszych narzędzi</h2>

            <div id="goal-adder-dialog" class="goal-creator-container" title='Wpisz nowy cel:'>
                <input type="text" name="goal" id="new-goal-name">
            </div>

            <button id="goal-adder-button" type="button" title="Create a new goal">+</button>

            <ul class="goals">
            </ul>
        </div>

        <footer>
            <a class="back-to-top" href="#myHeader">Powrót do góry</a>
            <p class="copyright">Copyright 2022 Paweł Bogdanowicz</p>
        </footer>
    </div>
</body>

</html>