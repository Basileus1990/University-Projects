<?php
//viewNames constants
const INDEX = 'index';
const CONTACT = 'contact';
const GOALTRACKER = 'goalTracker';
const GALLERY = 'gallery';
const FAV_IMAGES_GALLERY = 'favImagesGallery';
const IMAGE_SENT_RESULT = 'sentMessageResult';
const SEARCH = 'search';
const SEARCHRESULT = 'searchResult';
const ACCOUNT = 'account';
const ERROR404 = 'E404';

// Subpages addresses of the website
$routing = [
    '/' => INDEX,
    '/kontakt' => CONTACT,
    '/kalendarzyk' => GOALTRACKER,
    '/galeria' => GALLERY,
    '/ulubione-zdjecia' => FAV_IMAGES_GALLERY,
    '/konto' => ACCOUNT,
    '/wyszukiwarka' => SEARCH,
    '/wyszukiwarka/wyszukaj' => SEARCHRESULT
];
