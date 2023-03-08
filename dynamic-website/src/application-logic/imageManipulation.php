<?php

const ORIGINALDIR = '/originals/';
const THUMBNAILDIR = '/thumbnails/';
const WATERMARKDIR = '/watermarked/';

class SentImage
{
    function __construct($imageID, $image, $uploadDir)
    {
        $this->uploadDir = $uploadDir;
        $this->image = $image;

        if ($this->image['type'] === 'image/jpg' || $this->image['type'] === 'image/jpeg') {
            $this->image['name'] = $imageID . '.jpg';
            $this->newImg = imagecreatefromjpeg($this->image['tmp_name']);
        } elseif ($this->image['type'] === 'image/png') {
            $this->image['name'] = $imageID . '.png';
            $this->newImg = imagecreatefrompng($this->image['tmp_name']);
        }
    }

    function saveOriginalImage()
    {
        return move_uploaded_file($this->image['tmp_name'], $this->uploadDir . ORIGINALDIR . $this->image['name']);
    }

    function saveNewImage($directory)
    {
        if ($this->image['type'] === 'image/jpg' || $this->image['type'] === 'image/jpeg') {
            imagejpeg($this->newImg, $this->uploadDir . $directory . $this->image['name']);
        } elseif ($this->image['type'] === 'image/png') {
            imagepng($this->newImg, $this->uploadDir . $directory . $this->image['name']);
        }
    }

    function createWatermark($watermark)
    {
        $color = imagecolorallocate($this->newImg, 255, 255, 255);
        $posY = ceil(imagesy($this->newImg)) - 20;
        $fontSize = $posY / 5;
        $posX = ceil(imagesx($this->newImg) / 2) - strlen($watermark) * $fontSize / 3;
        putenv('GDFONTPATH=' . realpath('.'));
        imagettftext($this->newImg, $fontSize, 0, $posX, $posY, $color, 'fonts/arial', $watermark);
        $this->saveNewImage(WATERMARKDIR);
    }

    function createThumbnail()
    {
        define('THUMBNAIL_WIDTH', 200);
        define('THUMBNAIL_HEIGHT', 125);
        $this->newImg = imagescale($this->newImg, THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT);

        $this->saveNewImage(THUMBNAILDIR);
    }
}

// developer tool
function clearImages()
{
    $db = getDB();
    $db->images->deleteMany([]);
}

function saveSentImageInDatabase($id, $name, $title, $author, $authorID, $imagesPath)
{
    $db = getDB();
    $dbImage = [
        '_id' => $id,
        'name' => $name,
        'title' => $title,
        'author' => $author,
        'authorID' => $authorID,
        'imagesPath' => $imagesPath
    ];

    $db->images->insertOne($dbImage);
}

function getSavedImagesByPage($selectedPage, $imagesPerPage, $user)
{
    $userID = null;
    if ($user !== null) {
        $userID = $user['_id'];
    }
    $querry = [
        '$or' => [
            ['authorID' => null],
            ['authorID' => $userID],
        ]
    ];
    $options = [
        'skip' => ($selectedPage - 1) * $imagesPerPage,
        'limit' => $imagesPerPage
    ];

    $db = getDB();
    return $db->images->find($querry, $options);
}

function getSavedImagesByTitle($user, $title)
{
    $userID = null;
    if ($user !== null) {
        $userID = $user['_id'];
    }
    $querry = [
        '$or' => [
            ['authorID' => null],
            ['authorID' => $userID],
        ],
        "title" => new MongoDB\BSON\Regex($title)
    ];

    $db = getDB();
    return $db->images->find($querry);
}

function getSavedImageByID($id, $user)
{
    $userID = null;
    if ($user !== null) {
        $userID = $user['_id'];
    }
    $querry = [
        '$or' => [
            ['authorID' => null],
            ['authorID' => $userID],
        ],
        '_id' => $id
    ];

    $db = getDB();
    return $db->images->findOne($querry);
}

function getAmountOfImages($user)
{
    $userID = null;
    if ($user !== null) {
        $userID = $user['_id'];
    }
    $querry = [
        '$or' => [
            ['authorID' => null],
            ['authorID' => $userID],
        ]
    ];
    $db = getDB();
    return $db->images->count($querry);
}

function getAllImages()
{
    $db = getDB();
    return $db->images->find();
}
