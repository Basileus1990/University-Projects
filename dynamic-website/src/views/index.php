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

    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.6.0/jquery.min.js" integrity="sha512-894YE6QWD5I59HgZOGReFYm4dnWc1Qt5NtvYSaNcOP+u1T9qYdvdihz0PPSiiqn/+/3e7Jo4EaG7TubfWGUrMQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.13.2/jquery-ui.min.js" integrity="sha512-57oZ/vW8ANMjR/KQ6Be9v/+/h6bq9/l3f0Oc7vn6qMqyhvPd1cvKBRWWpzu0QoneImqr2SkmO4MSqU+RpHom3Q==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    <script src="scripts/self-development--areas.js" defer></script>
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
                <li class="nav-button active"><a href="#myHeader">Strona główna</a></li>
                <li class="nav-button"><a href="#how-to-begin">Jak zacząć?</a></li>
                <li class="nav-button"><a href="#self-development-tools">Pomocne Narzędzia</a></li>
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
            <section>
                <h2>Czym jest samorozwój?</h2>
                <p>Jest to pytanie, które powinien sobie zadać każdy człowiek pragnący osiągnąć maksymalną satysfakcję z życia. Samorozwój jest to proces, odbywający się w wielu wymiarach, między innymi rozwój personalny, duchowy czy zawodowy.
                    W efekcie prowadzi to człowieka do stawania się najlepszą wersją siebie. Zwiększa się znacznie satysfakcja i zadowolenie z życia.
                    Celem tej strony jest pomoc w rozpoczęciu przygody z samorozwojem.
                </p>

                <div id="self-improvement-areas">
                    <ul>
                        <li><a href="#tab1">Samorozwój Personalny</a></li>
                        <li><a href="#tab2">Samorozwój Duchowy</a></li>
                        <li><a href="#tab3">Samorozwój Zawodowy</a></li>
                    </ul>
                    <div id="tab1">
                        <p><strong>Samorozwój personalny</strong> polega na stawaniu się najlepszą możliwą wersją siebie. Robi się to poprzez zmianę swoich nawyków. Na przykład poprzez rozpoczęcie regularnych ćwiczeń, przejście na zdrową dietę czy wcześniejsze chodzenie spać
                            Wszystko to sprawia, że stajemy się coraz bardziej produktywni i szczęśliwi. O zmianie nawyków świetnie opowiada James Clear w książce "Atomowe Nawyki". Serdecznie polecamy lekturę: <a href="https://jamesclear.com/atomic-habits">"Atomowe Nawyki"</a>
                        </p>
                    </div>
                    <div id="tab2">
                        <p><strong>Samorozwój duchowy</strong> polega na skupieniu się na tym, co jest w środku nas. Celem tego jest to abyśmy się poczuli się dobrze z nami samymi. Pozwala to na zaakceptowanie swojego "ja". Jest to niezbędne do zostania szczęśliwą osobą. Jedną z metod tego typu rozwoju jest medytacja. Pozwala ona na zaznanie spokoju nawet w stresujących sytuacjach.<br>
                            Ale to nie musi być koniecznie medytacja. Może to być, tak jak wcześniej wspomnieliśmy cokolwiek, chociażby codzienne powtarzanie sobie za co jesteśmy wdzięczni. </p>
                    </div>
                    <div id="tab3">
                        <p><strong>Samorozwój zawodowy</strong> polega zaś na ciągłym podnoszeniu swoich kompetencji w branży, w której pracujemy. Może przyjąć wiele form, zależnie od dziedziny pracy. Może również czerpać z innych typów samorozwoju, gdyż one wzmacniając naszą produktywność i nastawienie powodują zwiększenie efektywności zawodowej.<br>
                            Przykładami samorozwoju zawodowego może być na przykład nauka nowego języka, albo przeczytanie książki dotyczącej dziedziny naszego zawodu. Dzięki temu rodzajowi samorozwoju możemy poczuć się spełnieni w swojej pracy i polepszymy swoją sytuację materialną.</p>
                    </div>
                </div>
            </section>

            <section id="how-to-begin">
                <h2>Jak zacząć?</h2>
                <p>Po pierwsze należy wybrać dziedziny które chcemy u siebie polepszyć. Może to być rozwój swojej pasji, zrzucenie zbędnych kilogramów czy nauka języka.
                    Ogólnie rzecz biorąc, cokolwiek. Ważne, aby to było coś, co szczerze chcemy poprawić.
                </p>
                <p>Następnie należy dokładnie zaplanować i zwizualizować swój cel.
                    Mimo to najważniejsze jest po prostu zacząć. Nie powiniśmy zastanawiać się zbyt długo nad rozpocząeciem działania, gdyż w czasie rozmyślań możemy stracić motywację.
                </p>
                <p>
                    Aby nie utracić motywacji warto również śledzić swoje postępy. Do tego polecamy użycie naszego kalendarzyka, do którego link można znaleźć poniżej. Dzięki niemu można łatwo śledzić ile dni trzymamy się nowego naywku, a w razie potknięcia się można zresetować ten licznik.
                </p>
            </section>

            <section id="self-development-tools">
                <h2>Narzędzia pomocne przy samorozwoju</h2>
                <ul>
                    <li>
                        <p>
                            Nasz <strong><a href="goal-tracker">Kalendarzyk</a></strong>
                        </p>
                    </li>
                    <li>
                        <p>
                            Warto również zobaczyć naszą galerię z plakatami o samorozwoju: <strong><a href="galeria">Galeria</a></strong>
                        </p>
                    </li>
                    <li>
                        <p>
                            Strona do planowania dnia na przykład: <strong><a href="https://todoist.com/">todoist.com</a></strong>
                        </p>
                    </li>
                    <li>
                        <p>
                            Książki o tematyce samorozwoju - pozwalają głębiej zagłębić się w ten temat
                        </p>
                    </li>
                </ul>
            </section>

            <section>
                <p>Masz jakieś pytania? Albo chciałbyć przekazać nam uwagę odnośnie naszej strony? Prosimy do nas napisać klikając w ten <strong><a href="kontakt">LINK</a></strong></p>
            </section>
        </div>

        <footer>
            <a class="back-to-top" href="#myHeader">Powrót do góry</a>
            <p class="copyright">Copyright 2022 Paweł Bogdanowicz</p>
        </footer>
    </div>
</body>

</html>