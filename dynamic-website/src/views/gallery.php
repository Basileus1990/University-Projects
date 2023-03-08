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
    <link rel="stylesheet" href="styles/magnific-popup.css">
    <link rel="stylesheet" href="styles/gallery.css">

    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.6.0/jquery.min.js" integrity="sha512-894YE6QWD5I59HgZOGReFYm4dnWc1Qt5NtvYSaNcOP+u1T9qYdvdihz0PPSiiqn/+/3e7Jo4EaG7TubfWGUrMQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    <script src="scripts/jquery.magnific-popup.min.js" defer></script>
    <script src="scripts/gallery.js" defer></script>
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
                <li class="nav-button active"><a href="#myHeader">Galeria</a></li>
                <li class="nav-button dropdown" tabindex="0">
                    Więcej
                    <ul class="dropdown-items">
                        <li class="nav-button"><a href="kontakt">Kontakt</a></li>
                        <li class="nav-button"><a href="kalendarzyk">Kalendarzyk</a></li>
                        <li class="nav-button"><a href="ulubione-zdjecia">Ulubione zdjęcia</a></li>
                        <li class="nav-button"><a href="wyszukiwarka">Wyszukiwarka</a></li>
                    </ul>
                </li>
            </ul>
        </nav>
        <?= !include 'account-info.php'; ?>
        <div class="website-content">
            <div class="gallery-content">
                <h2>Galeria o samorozwoju</h2>
                <form action="galeria" method="post">
                    <input type="text" name="save-selected" hidden value="save-selected" />
                    <div class="popup-gallery">
                        <?php foreach ($model['images-data'] as $image) : ?>
                            <div class="popup-image">
                                <p>
                                    <input name="saved-image-id[]" type="checkbox" value="<?= $image['_id'] ?>" <?php if (!empty($model['savedImagesID']) && in_array($image['_id'], $model['savedImagesID'])) : ?> checked <?php endif ?> />
                                    <?php if ($image['authorID'] !== null) : ?>Prywatne: <?php endif ?>
                                <?= $image['title'] ?>
                                </p>
                                <a href="<?= 'images/watermarked/' . $image['name'] ?>" title="<?= $image['title'] ?>">
                                    <img src="<?= 'images/thumbnails/' . $image['name'] ?>" width="200" height="125" alt="Nie udało się wczytać miniaturki" />
                                </a>
                                <p><?= $image['author'] ?></p>
                            </div>
                        <?php endforeach ?>
                    </div>
                    <input type="submit" class="save-selected-button" value="Zapisz wybrane" name="buttonSave">
                    <input type="submit" class="save-selected-button" value="Usuń wybrane" name="buttonDelete">
                </form>
                <div class="page-selector">
                    <a href="<?= 'galeria?page=' . ($model['page'] - 1) ?>" title="Poprzednia strona">&#60&#60&#60</a>
                    <span><?= $model['page'] ?></span>
                    <a href="<?= 'galeria?page=' . ($model['page'] + 1) ?>" title="Natępna strona">&#62&#62&#62</a>
                </div>
            </div>
            <div class="image-user-input">
                <form action="galeria" method="post" enctype="multipart/form-data">
                    <label for="#image">Wyślij zdjęcie:</label>
                    <input type="file" , name="sent-image" id="sent-image" accept=".jpg, .jpeg, .png" required />

                    <label for="#title">Tytuł:</label>
                    <input type="text" name="title" id="title" required>

                    <label for="#title">Autor:</label>
                    <input type="text" name="author" id="author" required <?php if ($model['user'] !== null) : ?> value="<?= $model['user']['login'] ?>" <?php endif ?>>

                    <label for="#watermark">Znak wodny:</label>
                    <input type="text" name="watermark" id="watermark" required>

                    <div>
                        <label for="#public-radio" <?php if ($model['user'] === null) : ?>hidden<?php endif ?>>Publiczne:</label>
                        <input type="radio" id="public-radio" name="visibility" value="public" checked <?php if ($model['user'] === null) : ?>hidden<?php endif ?>>
                    </div>
                    <div>
                        <label for="#private-radio" <?php if ($model['user'] === null) : ?>hidden<?php endif ?>>Prywatne:</label>
                        <input type="radio" id="private-radio" name="visibility" value="private" <?php if ($model['user'] === null) : ?>hidden<?php endif ?>>
                    </div>
                    <?php if ($model['user'] !== null) : ?>

                    <?php endif ?>

                    <button type="submit">Wyślij zdjęcie</button>
                </form>
            </div>
        </div>

        <footer>
            <a class="back-to-top" href="#myHeader">Powrót do góry</a>
            <p class="copyright">Copyright 2022 Paweł Bogdanowicz</p>
        </footer>
    </div>
</body>

</html>