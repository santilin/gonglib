angular.module('biosegura')
        .controller('contactosCtrl', ['$scope', '$resource', 'Contactos', 'NgTableParams', 'Notification', function ($scope, $resource, Contactos, NgTableParams, Notification) {
                Notification.clearAll();

                // textos que apareceran en las plantillas
                $scope.textos = {
                    tabla: 'Contactos',
                    elemento: 'Contacto'
                };
                // plantillas que usaremos en distintos sitios
                $scope.templates = {
                    modales: {
                        add: 'ng-templates/contactos/add.html',
                        edit: 'ng-templates/contactos/edit.html',
                        del: 'ng-templates/contactos/delete.html'
                    }
                };

                // formularios que vamos a necesitar
                $scope.addingForm = {};
                $scope.editForm = {};
                $scope.delForm = {};

                // columnas/campos de la BBDD y su configuracion en el Fontend
                $scope.cols = [
                    // si queremos que el campo sea de tipo numérico añadimos tipo: 'number' pero si ademas queremos que pueda ser decimal o que vaya
                    // step a step tenemos que añadir la propiedad STEP, si no lo hacemos se mostrará como un step normal de 1 en 1
                    // mientras que si lo añadimos se puede mostrar un step que avance de 0.5 en 0.5, por ejemplo
                    {field: "CODIGO", title: "Código", filter: {CODIGO: 'text'}, show: true, sortable: "CODIGO", tipo: 'text', required: true},
                    {field: "NOMBRE", title: "Nombre", filter: {NOMBRE: 'text'}, show: true, sortable: "NOMBRE", tipo: 'text', required: true},
                    {field: "CIF", title: "Cif/nif", filter: {CIF: 'text'}, show: true, sortable: "CIF", tipo: 'text', required: true},
                    {field: "DIRECCION", title: "Dirección", filter: {DIRECCION: 'text'}, show: true, sortable: "DIRECCION", tipo: 'text', required: true},
                    {title: "Acciones", show: true, botones: true}
                ];


                // configuracion de NG-TABLE
                $scope.tableParams = new NgTableParams({
                    page: 1,
                    count: 5
                }, {
                    counts: [5, 10, 20, 50, 100],
                    paginationMaxBlocks: 6,
                    getData: function (params) {
                        return Contactos.getFiltered(params.parameters()).$promise.then(function (respuesta) {
                            params.total(respuesta.total);
                            return respuesta.data;
                        });
                    }
                });


                /**************************************************************
                 /**************************************************************
                 /********************   FUNCIONES   ***************************
                 /**************************************************************
                 /**************************************************************


                 /*
                 * AÑADIR AGENTES
                 */
                $scope.add_Item = function (form) {
					console.log('Creando');
                    Notification.clearAll();

                    Contactos.create(form).$promise
                            .then(function (result) {
                                // añadimos visualmente el nuevo elemento añadido
                                $scope.tableParams.data.unshift(result);
                                // eliminamos visualmente el ultimo elemento de la lista
                                $scope.tableParams.data.pop();
                                // limpiamos el formulario de creacion de usuarios
                                $scope.addingForm = {};
                                // ocultamos el modal
                                $(".modal").modal("hide");
                                Notification.success({message: $scope.textos.elemento + ' añadido correctamente'});
                            })
                            .catch(function (error) {
                                $.each(error.data, function(index, msg) {
                                    Notification.error({message: msg});
                                });
                            });
                };

                // reseteamos los datos del formulario de Crear Contacto
                $scope.clean_addF = function () {
                    $scope.addingForm = {};
                };

                $scope.nada = function () {
                    console.log(2);
                };


                /*
                 * ELIMINAR AGENTES
                 */
                $scope.eliminar = function (contacto) {
                    $scope.delForm = contacto;
                };

                $scope.del_Item = function () {
                    Notification.clearAll();

                    Contactos.remove($scope.delForm.id).$promise
                            .then(function (result) {
                                $scope.tableParams.reload();
                                $(".modal").modal("hide");
                                Notification.success({message: $scope.textos.elemento + ' eliminado correctamente'});
                            })
                            .catch(function (error) {
                                Notification.error({message: error.data});
                            });
                };

                // funcion para mostrar el dato en el modal de eliminación (solo mostramos los datos que aparecen en el formulario)
                $scope.delFormShow = function (campo) {
                    for (var i in $scope.cols) {
                        if (campo === $scope.cols[i].field)
                            return true;
                    }
                    return false;
                };


                $scope.editar = function (item) {
                    Contactos.edit(item).$promise
                            .then(function (result) {
                                $scope.editForm = result;
                            })
                            .catch(function (error) {
                                error.data.forEach(function(msg, index) {
                                    Notification.error({message: msg});
                                });
                            });
                };


                $scope.edit_Item = function () {
                    Notification.clearAll();

                    Contactos.update($scope.editForm).$promise
                            .then(function (result) {
                                $scope.tableParams.reload();
                                $scope.nElementos = ['mensaje1','mensaje2'];
                                $(".modal").modal("hide");
                                Notification.success({message: $scope.textos.elemento + ' modificado correctamente'});
                            })
                            .catch(function (error) {
                                error.data.forEach(function(msg, index) {
                                    Notification.error({message: msg});
                                });
                            });
                };
            }]);

