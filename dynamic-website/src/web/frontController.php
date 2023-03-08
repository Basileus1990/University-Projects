<?php

require_once '../routing.php';
require_once '../dispatcher.php';
require_once '../controllers.php';
require_once '../../vendor/autoload.php';

session_start();

$userAction = $_GET['action'];

dispatch($routing, $userAction);
