//@holadoc Angular/tooltip  Para usar tooltip a cada elemento html hay que añadirle los atributos '<a bootstrap-tooltip title="Este es el tooltip"></a>' y añadir la directiva bootstrapTooltip
// registers native Twitter Bootstrap tooltips
angular.module('biosegura').directive('bootstrapTooltip', function () {
    return function (scope, element, attrs) {
        attrs.$observe('title', function (title) {
            // Destroy any existing tooltips (otherwise new ones won't get initialized)
            element.tooltip('destroy');
            // Only initialize the tooltip if there's text (prevents empty tooltips)
            if (jQuery.trim(title))
                element.tooltip();
        });
        element.on('$destroy', function () {
            element.tooltip('destroy');
            delete attrs.$$observers['title'];
        });
    };
});
