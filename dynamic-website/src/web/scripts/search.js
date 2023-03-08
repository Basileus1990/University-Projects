$(function() {
    let imageResultDiv = $('#search-result');
    let searchBar = $('#search-bar');
    searchBar.keyup(() => {
        let search = searchBar.val();
        if(search === '') {
            imageResultDiv.html('');
            return;
        }
        $.ajax({
            url: 'wyszukiwarka/wyszukaj',
            data: {search: search},
            success: function(data) {
                imageResultDiv.html(data);
            },
          });
    });
});