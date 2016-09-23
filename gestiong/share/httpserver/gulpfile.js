var elixir = require('laravel-elixir');

/*
 |--------------------------------------------------------------------------
 | Elixir Asset Management
 |--------------------------------------------------------------------------
 |
 | Elixir provides a clean, fluent API for defining some basic Gulp tasks
 | for your Laravel application. By default, we are compiling the Sass
 | file for our application, as well as publishing vendor resources.
 |
 */

elixir.config.css.autoprefix.options = {
    cascade : true,
    browsers: ['last 2 versions', '> 1%']
};

elixir(function (mix) {
    mix.sass('app.scss');
    mix.browserSync({
        proxy: 'app.aseaga.com'
    });
    
    mix.scriptsIn('resources/assets/js/ngApp', 'app/js/all.js');
    mix.scriptsIn('resources/assets/js/custom', 'app/js/biosegura.min.js');
});
