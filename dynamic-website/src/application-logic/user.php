<?php

function login($login, $password)
{
}

require 'database.php';

function registerUser($login, $email, $password, $sessionID)
{
    $user = [
        'sessionID' => $sessionID,
        'login' => $login,
        'email' => $email,
        'password' => password_hash($password, PASSWORD_DEFAULT)
    ];

    $db = getDB();
    $db->users->insertOne($user);
}

function loginUser($user, $sessionID)
{
    $querry = ['_id' => $user['_id']];
    $user['sessionID'] = $sessionID;
    $db = getDB();
    $db->users->replaceOne($querry, $user);
}

function logoutUser($sessionID)
{
    $user = getLoggedUser($sessionID);
    $user['sessionID'] = null;
    $querry = ['_id' => $user['_id']];
    $db = getDB();
    $db->users->replaceOne($querry, $user);
}

function getLoggedUser($sessionID)
{
    $db = getDB();
    return $db->users->findOne(['sessionID' => $sessionID]);
}

function getUserWithGivenLogin($login)
{
    $db = getDB();
    return $db->users->findOne(['login' => $login]);
}

function checkIfPasswordMatches($password, $user)
{
    return password_verify($password, $user['password']);
}

//developer tool
function clearUsers()
{
    $db = getDB();
    $db->users->deleteMany([]);
}
