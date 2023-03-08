<?php if ($model['user'] !== null) : ?>
    <div class="account-info">
        <span>Zalogowany jako: <?= $model['user']['login'] ?></span>
        <form action="konto" method="post">
            <input type="text" name="logreg" hidden value="logout">
            <button type="submit">Wyloguj</button>
        </form>
    </div>
<?php endif ?>

<?php if ($model['user'] === null) : ?>
    <div class="account-info not-loged">
        <form action="konto" method="get">
            <button type="submit">Zaloguj</button>
            <button type="submit">Zarejestruj</button>
        </form>
    </div>
<?php endif ?>