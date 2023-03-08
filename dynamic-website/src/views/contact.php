<!doctype html>
<html>

<head lang="pl-PL">
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <meta name="description" content="Strona o samorozwoju. O tym jak zacząć i z elementami ułatwiającymi rozpoczęcie tej pięknej przygody">
    <title>Samorozwój</title>
    <link rel="icon" type="image/x-icon" href="images/favicon.ico">

    <link rel="stylesheet" href="styles/header.css">
    <link rel="stylesheet" href="styles/nav.css">
    <link rel="stylesheet" href="styles/style.css">
    <link rel="stylesheet" href="styles/content.css">

    <script src="scripts/form-saver.js" defer></script>
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
                <li class="nav-button active"><a href="#myHeader">Kontakt</a></li>
                <li class="nav-button dropdown" tabindex="0">
                    Więcej
                    <ul class="dropdown-items">
                        <li class="nav-button"><a href="kalendarzyk">Kalendarzyk</a></li>
                        <li class="nav-button"><a href="galeria">Galeria</a></li>
                        <li class="nav-button"><a href="ulubione-zdjecia">Ulubione zdjęcia</a></li>
                        <li class="nav-button"><a href="wyszukiwarka">Wyszukiwarka</a></li>
                    </ul>
                </li>
            </ul>
        </nav>
        <?= !include 'account-info.php'; ?>
        <div class="website-content">
            <h2>Wyślij nam swoją wiadomość odnośnie samorozwoju albo naszego serwisu.</h2>
            <form class="message-form" action="odbierz.php" method="post">
                <div class="">
                    <div class="input-box">
                        <label for="name">Imię: </label>
                        <input id="name" name="name" type="text">
                    </div>

                    <div class="input-box">
                        <label for="surname">Nazwisko: </label>
                        <input id="surname" name="surname" type="text">
                    </div>

                    <div class="input-box">
                        <label for="email">E-mail: </label>
                        <input id="email" name="email" type="email">
                    </div>

                    <div class="input-box">
                        <label for="age">Wiek: </label>
                        <input id="age" name="age" type="number">
                    </div>

                    <div class="input-box">
                        <div>
                            <label for="sex-male">Mężczyzna: </label>
                            <input id="sex-male" name="sex" type="radio" value="male">
                        </div>
                        <div>
                            <label for="sex-female">Kobieta: </label>
                            <input id="sex-female" name="sex" type="radio" value="female">
                        </div>
                        <div>
                            <label for="sex-other">Inna: </label>
                            <input id="sex-other" name="sex" type="radio" value="other">
                        </div>
                    </div>

                    <div class="input-box">
                        <label for="message-type">Typ wiadomości:</label>
                        <select id="message-type" name="message-type">
                            <option>Pytanie</option>
                            <option>Propozycja</option>
                            <option>Inne</option>
                        </select>
                    </div>

                    <div class="input-box message-buttons">
                        <button type="submit">Wyślij</button>
                        <button type="reset">Resetuj</button>
                    </div>
                </div>
                <div class="message input-box">
                    <label for="message">Wiadomość:</label>
                    <textarea id="message" name="message"></textarea>
                </div>
            </form>

        </div>
        <footer>
            <a class="back-to-top" href="#myHeader">Powrót do góry</a>
            <p class="copyright">Copyright 2022 Paweł Bogdanowicz</p>
        </footer>
    </div>
</body>

</html>