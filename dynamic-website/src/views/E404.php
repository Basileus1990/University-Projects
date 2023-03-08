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
                <li class="nav-button active"><a href="#myHeader">ERROR 404</a></li>
                <li class="nav-button dropdown" tabindex="0">
                    Więcej
                    <ul class="dropdown-items">
                        <li class="nav-button"><a href="kontakt">Kontakt</a></li>
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
            <h1>ERROR 404</h1>
            <h2>Nie znaleziono zasobu!</h2>

            <footer>
                <a class="back-to-top" href="#myHeader">Powrót do góry</a>
                <p class="copyright">Copyright 2022 Paweł Bogdanowicz</p>
            </footer>
        </div>
</body>

</html>