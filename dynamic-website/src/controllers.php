<?php
define('STATUS_MESSAGE', 'image-status-message');
define('IMAGES_DATA', 'images-data');
define('IMAGES_PER_PAGE', 5);
include 'application-logic/user.php';

function checkLoginStatus(&$model)
{
    $model['user'] = getLoggedUser(session_id());
}

function index(&$model)
{
    checkLoginStatus($model);
    return INDEX;
}

function contact(&$model)
{
    checkLoginStatus($model);
    return CONTACT;
}

function goalTracker(&$model)
{
    checkLoginStatus($model);
    return GOALTRACKER;
}

function gallery(&$model)
{
    define('SENT_IMAGE_KEY', 'sent-image');
    define('UPLOAD_DIR', getcwd() . '/images');

    checkLoginStatus($model);
    if (!isset($_SESSION['savedImagesID'])) {
        $_SESSION['savedImagesID'] = [];
    }
    $model['goBackLink'] = 'galeria';
    if ($_SERVER['REQUEST_METHOD'] === 'GET') {
        require 'application-logic/imageManipulation.php';
        $page = null;
        if (empty($_GET['page'])) {
            $page = 1;
        } elseif ($_GET['page'] > 0 && $_GET['page'] <= ceil(getAmountOfImages($model['user']) / IMAGES_PER_PAGE)) {
            $page = $_GET['page'];
        } elseif ($_GET['page'] <= 0) {
            $page = 1;
        } elseif ($_GET['page'] > ceil(getAmountOfImages($model['user']) / IMAGES_PER_PAGE)) {
            $page = intval(ceil(getAmountOfImages($model['user']) / IMAGES_PER_PAGE));
        }

        $model['page'] = $page;
        $model[IMAGES_DATA] = getSavedImagesByPage($page, IMAGES_PER_PAGE, $model['user']);
        if (!empty($_SESSION['savedImagesID'])) {
            $model['savedImagesID'] = $_SESSION['savedImagesID'];
        }

        return GALLERY;
    } elseif (isset($_POST['save-selected']) && !isset($_POST['saved-image-id'])) {
        $model[STATUS_MESSAGE] = 'Nie zaznaczano żadnego zdjęcia';
        return IMAGE_SENT_RESULT;
    } elseif (isset($_POST['save-selected']) && isset($_POST['buttonSave'])) {
        if (empty($_SESSION['savedImagesID'])) {
            $_SESSION['savedImagesID'] = [];
        }
        foreach ($_POST['saved-image-id'] as $savedImageID) {
            if (!in_array($savedImageID, $_SESSION['savedImagesID'])) {
                array_push($_SESSION['savedImagesID'], $savedImageID);
            }
        }

        $model[STATUS_MESSAGE] = 'Zapisanie się powiodło';
        return IMAGE_SENT_RESULT;
    } elseif (isset($_POST['save-selected']) && isset($_POST['buttonDelete'])) {
        if (empty($_SESSION['savedImagesID'])) {
            $_SESSION['savedImagesID'] = [];
        }
        foreach ($_POST['saved-image-id'] as $savedImageID) {
            if (in_array($savedImageID, $_SESSION['savedImagesID'])) {
                unset($_SESSION['savedImagesID'][array_search($savedImageID, $_SESSION['savedImagesID'])]);
            }
        }

        $model[STATUS_MESSAGE] = 'Usuwanie się powiodło';
        return IMAGE_SENT_RESULT;
    } elseif (!isset($_FILES[SENT_IMAGE_KEY])) {
        $model[STATUS_MESSAGE] = 'Wystąpił błąd. Przepraszamy';
        return IMAGE_SENT_RESULT;
    } elseif ($_FILES[SENT_IMAGE_KEY]['error'] != 0) {
        $model[STATUS_MESSAGE] = 'Nie wybrano żadnego pliku';
        return IMAGE_SENT_RESULT;
    } elseif (!($_FILES[SENT_IMAGE_KEY]['type'] === 'image/jpeg' || $_FILES[SENT_IMAGE_KEY]['type'] === 'image/jpg'  || $_FILES[SENT_IMAGE_KEY]['type'] === 'image/png') && $_FILES[SENT_IMAGE_KEY]['size'] > 1000000) {
        $model[STATUS_MESSAGE] = 'Zły typ pliku i obraz jest za duży. Maksymalny rozmiar to 1MB';
        return IMAGE_SENT_RESULT;
    } elseif (!($_FILES[SENT_IMAGE_KEY]['type'] === 'image/jpeg' || $_FILES[SENT_IMAGE_KEY]['type'] === 'image/jpg'  || $_FILES[SENT_IMAGE_KEY]['type'] === 'image/png')) {
        $model[STATUS_MESSAGE] = 'Zły typ pliku';
        return IMAGE_SENT_RESULT;
    } elseif ($_FILES[SENT_IMAGE_KEY]['size'] > 1000000) {
        $model[STATUS_MESSAGE] = 'Obraz za jest duży. Maksymalny rozmiar to 1MB';
        return IMAGE_SENT_RESULT;
    }

    $imageID = new MongoDB\BSON\ObjectID();
    require 'application-logic/imageManipulation.php';
    $imageManipulator = new SentImage($imageID, $_FILES[SENT_IMAGE_KEY], UPLOAD_DIR);
    if (!$imageManipulator->saveOriginalImage()) {
        $model[STATUS_MESSAGE] = 'Przesłanie obrazu się nie powiodło';
        return IMAGE_SENT_RESULT;
    }

    $imageManipulator->createWatermark($_POST['watermark']);
    $imageManipulator->createThumbnail();

    $authorID = null;
    if ($model['user'] !== null && $_POST['visibility'] === 'private') {
        $authorID = $model['user']['_id'];
    }
    saveSentImageInDatabase($imageID, $imageManipulator->image['name'], $_POST['title'], $_POST['author'], $authorID, UPLOAD_DIR);

    $model[STATUS_MESSAGE] = 'Przesłanie obrazu się powiodło';
    return IMAGE_SENT_RESULT;
}

function favImagesGallery(&$model)
{
    checkLoginStatus($model);
    if (!isset($_SESSION['savedImagesID'])) {
        $_SESSION['savedImagesID'] = [];
    }
    $model['goBackLink'] = 'ulubione-zdjecia';
    if ($_SERVER['REQUEST_METHOD'] === 'GET') {
        if (!isset($_SESSION['savedImagesID'])) {
            $model[STATUS_MESSAGE] = 'Nie dodano żadnych ulubionych zdjęć';
            return IMAGE_SENT_RESULT;
        }

        require 'application-logic/imageManipulation.php';
        $page = null;
        if (empty($_GET['page'])) {
            $page = 1;
        } elseif ($_GET['page'] > 0 && $_GET['page'] <= ceil(count($_SESSION['savedImagesID']) / IMAGES_PER_PAGE)) {
            $page = $_GET['page'];
        } elseif ($_GET['page'] <= 0) {
            $page = 1;
        } elseif ($_GET['page'] > ceil(count($_SESSION['savedImagesID']) / IMAGES_PER_PAGE)) {
            $page = intval(ceil(count($_SESSION['savedImagesID']) / IMAGES_PER_PAGE));
        }

        $model['page'] = $page;
        $model[IMAGES_DATA] = [];
        $i = -1;
        foreach ($_SESSION['savedImagesID'] as $id) {
            $i++;
            if ($i >= IMAGES_PER_PAGE * $page) {
                break;
            } elseif ($i < IMAGES_PER_PAGE * ($page - 1)) {
                continue;
            }
            array_push($model[IMAGES_DATA], getSavedImageByID(new MongoDB\BSON\ObjectID($id), $model['user']));
        }
        return FAV_IMAGES_GALLERY;
    } elseif (!empty($_POST['save-selected']) && !empty($_POST['buttonDelete'])) {
        if (empty($_SESSION['savedImagesID'])) {
            $_SESSION['savedImagesID'] = [];
        }
        foreach ($_POST['saved-image-id'] as $savedImageID) {
            if (in_array($savedImageID, $_SESSION['savedImagesID'])) {
                unset($_SESSION['savedImagesID'][array_search($savedImageID, $_SESSION['savedImagesID'])]);
            }
        }

        $model[STATUS_MESSAGE] = 'Usuwanie się powiodło';
        return IMAGE_SENT_RESULT;
    }

    return ERROR404;
}

function account(&$model)
{
    checkLoginStatus($model);
    $model['goBackLink'] = '';
    if ($_SERVER['REQUEST_METHOD'] === 'GET') {
        return ACCOUNT;
    }
    if ($_POST['logreg'] === 'login') {
        if (empty($_POST['login']) || empty($_POST['password'])) {
            $model[STATUS_MESSAGE] = 'Należy wpisać login i hasło!';
            return IMAGE_SENT_RESULT;
        }

        $login = $_POST['login'];
        $password = $_POST['password'];
        $user = getUserWithGivenLogin($login);
        if ($user === null) {
            $model[STATUS_MESSAGE] = 'Nie istnieje konto o takim loginie';
            return IMAGE_SENT_RESULT;
        } elseif (!checkIfPasswordMatches($password, $user)) {
            $model[STATUS_MESSAGE] = 'Niepoprawne hasło';
            return IMAGE_SENT_RESULT;
        }

        loginUser($user, session_id());

        $_SESSION['userID'] = $user['_id'];
        $model[STATUS_MESSAGE] = 'Pomyślnie zalogowano';
        return IMAGE_SENT_RESULT;
    } elseif ($_POST['logreg'] === 'register') {
        if (empty($_POST['login']) || empty($_POST['password']) || empty($_POST['email'])) {
            $model[STATUS_MESSAGE] = 'Należy wpisać login, email i hasło!';
            return IMAGE_SENT_RESULT;
        }

        $login = $_POST['login'];
        $email = $_POST['email'];
        $password = $_POST['password'];
        $passwordRepeat = $_POST['password-repeat'];
        $user = getUserWithGivenLogin($login);
        if ($user !== null) {
            $model[STATUS_MESSAGE] = 'Istnieje konto o takim loginie';
            return IMAGE_SENT_RESULT;
        } elseif ($password !== $passwordRepeat) {
            $model[STATUS_MESSAGE] = 'Podane hasła nie są takie same';
            return IMAGE_SENT_RESULT;
        }

        registerUser($login, $email, $password, session_id());

        $model[STATUS_MESSAGE] = 'Pomyślnie zarejestrowano';
        return IMAGE_SENT_RESULT;
    } elseif ($_POST['logreg'] === 'logout') {
        logoutUser(session_id());

        $model['user'] = null;
        $model[STATUS_MESSAGE] = 'Pomyślnie wylogowano';
        $model['goBackLink'] = 'konto';
        return IMAGE_SENT_RESULT;
    }

    return ACCOUNT;
}

function search(&$model)
{
    checkLoginStatus($model);
    return SEARCH;
}

function searchResult(&$model)
{
    checkLoginStatus($model);
    if ($_SERVER['REQUEST_METHOD'] === 'GET') {
        require 'application-logic/imageManipulation.php';

        $search = $_GET['search'];
        $model[IMAGES_DATA] = getSavedImagesByTitle($model['user'], $search)->toarray();

        $isEmpty = true;
        // foreach ($model[IMAGES_DATA] as $image) {
        //     $isEmpty = false;
        //     break;
        // }
        if (!count($model[IMAGES_DATA])) {
            $model[IMAGES_DATA] = [];
        }
        return SEARCHRESULT;
    }
    return SEARCHRESULT;
}
