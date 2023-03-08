<?php if (isset($model['images-data']) && $model['images-data'] !== []) : ?>

    <div class="popup-gallery">
        <?php foreach ($model['images-data'] as $image) : ?>
            <div class="popup-image">
                <p>
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

<?php endif ?>