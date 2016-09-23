

angular.module('biosegura')
    .service('propiedadesCompartidas', function () {
        var idImportacion = '';

        return {
            getIdImportacion: function () {
                return idImportacion;
            },
            setIdImportacion: function(value) {
                idImportacion = value;
            }
        };
    });
