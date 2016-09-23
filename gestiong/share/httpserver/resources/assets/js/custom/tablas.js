(function ($) {
    // seleccionar la fila sobre la que hacemos click
    $('body').on('click', 'table td, table td div :not(button)', function () {
        if ($(this).find('button').length === 0) {
            $(this).parent().toggleClass('selected_row');
            $(this).parent().siblings().removeClass('selected_row');
        }
    });

    // colapsar/expandir la tabla
    $('body').on('click', '.collapse-link', function () {
        var $BOX_PANEL = $(this).closest('.x_panel'),
                $ICON = $(this).find('i'),
                $BOX_CONTENT = $BOX_PANEL.find('.x_content, .filtros');

        // fix for some div with hardcoded fix class
        if ($BOX_PANEL.attr('style')) {
            $BOX_CONTENT.slideToggle(400, function () {
                $BOX_PANEL.removeAttr('style');
            });
        } else {
            $BOX_CONTENT.slideToggle(400);
            $BOX_PANEL.css('height', 'auto');
        }

        $ICON.toggleClass('fa-chevron-up fa-chevron-down');
    });
    
    // oculta el tooltip del boton al hacer clic
    $('body').on('click', 'button', function () {
        $(this).tooltip('hide');
    });
})(jQuery);