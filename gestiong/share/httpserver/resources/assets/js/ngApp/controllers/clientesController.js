angular.module('biosegura')
        .controller('clientesCtrl', ['$scope', '$resource', 'Clientes', 'NgTableParams', 'Notification', function ($scope, $resource, Clientes, NgTableParams, Notification) {
                Notification.clearAll();

                // textos que apareceran en las plantillas
                $scope.textos = {
                    tabla: 'Clientes',
                    elemento: 'Cliente'
                };
                // plantillas que usaremos en distintos sitios
                $scope.templates = {
                    modales: {
                        add: 'ng-templates/crud_general/modales/add.html',
                        edit: 'ng-templates/crud_general/modales/edit.html',
                        del: 'ng-templates/crud_general/modales/delete.html'
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
                    {field: "NOMBRE", title: "Nombre", filter: {nombre: 'text'}, show: true, sortable: "nombre", tipo: 'text', required: true},
                    {title: "Acciones", show: true, botones: true}
                ];
                
                
                $scope.colsEdit = [
                    {field: "codigo_agropelayo", title: "#AgroPelayo", filter: {codigo_agropelayo: 'text'}, show: true, sortable: "codigo_agropelayo", tipo: 'text', required: true},
                    {field: "codigo_tib", title: "#Tib", filter: {codigo_tib: 'text'}, show: true, sortable: "codigo_tib", tipo: 'text', required: true},
                    {field: "codigo_reale", title: "#Reale", filter: {codigo_reale: 'text'}, show: true, sortable: "codigo_reale", tipo: 'text', required: true},
                    {field: "tipo", title: "Tipo", filter: {Tipo: 'number'}, show: false, sortable: "tipo", tipo: 'text', required: true},
                    {field: "nombre", title: "Nombre", filter: {nombre: 'text'}, show: true, sortable: "nombre", tipo: 'text', required: true},
                    {field: "irpf", title: "IRPF", filter: {irpf: 'text'}, show: true, sortable: "irpf", tipo: 'text', required: true},
//                    {field: "irpf", title: "IRPF", filter: {irpf: 'text'}, show: true, sortable: "irpf", tipo: 'number', step: '0.10', required: true},
                    {field: "cif_nif", title: "CIF/NIF", filter: {cif_nif: 'text'}, show: true, sortable: "cif_nif", tipo: 'text', required: true},
                    {field: "cuenta_banco", title: "IBAN", filter: {cuenta_banco: 'text'}, show: true, sortable: "cuenta_banco", tipo: 'text', required: true},
                    {field: "direccion", title: "Dirección", filter: {direccion: 'text'}, show: true, sortable: "direccion", tipo: 'text', required: true},
                    {field: "localidad", title: "Localidad", filter: {localidad: 'text'}, show: true, sortable: "localidad", tipo: 'text', required: true},
                    {field: "cp", title: "C.P", filter: {cp: 'text'}, show: true, sortable: "cp", tipo: 'text', required: true},
                    {field: "provincia", title: "Provincia", filter: {provincia: 'text'}, show: true, sortable: "provincia", tipo: 'text', required: false},
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
                        return Clientes.getFiltered(params.parameters()).$promise.then(function (respuesta) {
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
                $scope.add_Item = function () {
                    Notification.clearAll();
                    
                    Clientes.create($scope.addingForm).$promise
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

                // reseteamos los datos del formulario de Crear Cliente
                $scope.clean_addF = function () {
                    $scope.addingForm = {};
                };

                /*
                 * ELIMINAR AGENTES
                 */
                $scope.eliminar = function (agente) {
                    $scope.delForm = agente;
                };

                $scope.del_Item = function () {
                    Notification.clearAll();
                    
                    Clientes.remove($scope.delForm.id).$promise
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
                    Clientes.edit(item).$promise
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
                    
                    Clientes.update($scope.editForm).$promise
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

