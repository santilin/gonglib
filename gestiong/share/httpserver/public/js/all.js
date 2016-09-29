angular.module("biosegura", ["ngRoute", "ngCookies", "ngResource", "ngTable", "ngAnimate", "ui-notification"])
        .config(function ($routeProvider) {
            $routeProvider
                    .when("/login", {
                        controller: "loginCtrl",
                        templateUrl: "ng-templates/login/loginForm.html",
                        authenticated: false
                    })
                    .when("/mi-panel", {
                        templateUrl: "ng-templates/main/index.html",
                        authenticated: true
                    })
                    .when("/mi-perfil", {
                        controller: "loginCtrl",
                        templateUrl: "ng-templates/usuario/gestion_usuario.html",
                        authenticated: true
                    })
                    .when("/usuarios", {
                        controller: "usuariosCtrl",
                        templateUrl: "ng-templates/crud_general/index.html",
                        authenticated: true
                    })
                    .when("/", {
                        controller: "contactosCtrl",
                        templateUrl: "ng-templates/crud_general/index.html",
                        authenticated: false
                    })
                    .when("/contratos", {
                        controller: "contratosCtrl",
                        templateUrl: "ng-templates/crud_general/index.html",
                        authenticated: false
                    })
                    .when("/ramos", {
                        controller: "ramosCtrl",
                        templateUrl: "ng-templates/crud_general/index.html",
                        authenticated: true
                    })
                    .when("/importaciones", {
                        controller: "importacionesCtrl",
                        templateUrl: "ng-templates/crud_general/index.html",
                        authenticated: true
                    })
                    .when("/comisiones", {
                        controller: "importacionesCtrl",
                        templateUrl: "ng-templates/importacion/index.html",
                        authenticated: true
                    })
                    // ERRORES
                    .otherwise({
                        templateUrl: "ng-templates/errors/404.html",
                        authenticated: false
                    });
        })

        .run(['$rootScope', "$location", "User", function ($rootScope, $location, User) {
                $rootScope.usuario = User.getUserObject() || {};

                $rootScope.$on("$routeChangeStart", function (event, next, current) {
                    // si intento ir a una pagina que requiere autenticacion y no lo estoy > me manda a '/'
                    if (next.$$route.authenticated) {
                        if (!User.getAuthStatus()) {
                            $location.path('/');
                        }
                    }
                    // si intento ir a '/' y ya estoy autenticado > me manda a '/mi-panel'
                    if (next.$$route.originalPath === '/') {
                        if (User.getAuthStatus()) {
//                            $location.path('/mi-panel');
                            $location.path('/contactos');
                        }
                    }
                });
            }])

        .config(function (NotificationProvider) {
            NotificationProvider.setOptions({
                delay: 15000,
                startTop: 20,
                startRight: 10,
                verticalSpacing: 20,
                horizontalSpacing: 20,
                positionX: 'right',
                positionY: 'bottom',
                templateUrl: "ng-templates/notificaciones/notification.html"
            });
        });               

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


angular.module('biosegura')
        .controller('loginCtrl', ['$rootScope', '$scope', '$location', '$q', 'User', 'Notification', function ($rootScope, $scope, $location, $q, User, Notification) {
                Notification.clearAll();
        
                // FUNCIONALIDAD PARA LOGARSE EN LA APP
                var inicioSesion = $q.defer();

                inicioSesion.promise.then(function (usuario) {
                    $rootScope.usuario = usuario;
                    $location.path('/mi-panel');
                });


                $scope.login_mensaje = null;

                $scope.logarse = function () {
                    var data = {email: $scope.login.email, password: $scope.login.clave};

                    var usr = User.doLogin(data)
                            .then(function (usr) {
                                inicioSesion.resolve(usr.data);
                            }).catch(function (error) {
                        $scope.login_mensaje = error.data;
                    });
                };
                
                
                // FUNCIONALIDADES ADICIONALES PARA CONTROL DEL USUARIO
//                $scope.msjError = function(){
//                    Notification.error({message: 'Mensaje de error', delay: 30000, title: 'Error' });
//                };
//                $scope.msjWarning = function(){};
//                $scope.msjSuccess = function(){};
            }]);


angular.module('biosegura')
        .controller('mainCtrl', ['$scope', function ($scope) {
                $scope.AppTitle = "Biosegura";
                $scope.volverAtras = function () {
                    window.history.back();
                };
            }]); 

angular.module('biosegura')
        .controller('navCtrl', ['$rootScope', '$scope', '$location', 'User', function ($rootScope, $scope, $location, User) {
                // seleccionar el enlace que está activo para cambiar su visualizacion
                var url = $location.path();
                $scope.linkActivo = url.slice(1,url.length);
                $scope.activarLink = function(enlace){
                    $scope.linkActivo = enlace;
                };

                $scope.registrado = function () {
                    return User.getAuthStatus();
                };

                $scope.menuAdmin = function () {
                    return User.isAdmin();
                };
                $scope.menuConta = function () {
                    return User.isConta();
                };
                $scope.menuCoord = function () {
                    return User.isCoord();
                };

                $scope.mostrarGraficas = function () {
                    return User.isAdmin() || User.isCoord();
                };

                $scope.logout = function () {
                    $rootScope.usuario = null;
                    User.doUserLogout();
                };
                
            }]); 

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

angular.module("biosegura")
    .directive("hmLeftNav", function() {
        return {
            restrict: "E",
            controller: "navCtrl",
            templateUrl: "ng-templates/nav/nav-left.html"
        };
    });

angular.module("biosegura")
    .directive("hmTopNav", function() {
      return {
        restrict: "E",
        templateUrl :"ng-templates/nav/nav-top.html"
      };
    });

angular.module('biosegura')
        .factory('Clientes', ['$resource', function ($resource) {
                return {
                    getAll: function (data) {
                        return $resource('/api/cliente/:data', data).query();
                    },
                    getFiltered: function (filters) {
                        //@holadoc Angular/ngResource  $resource("../rest/api"}).get(); return an object.  $resource("../rest/api").query(); return an array.
                        return $resource('/api/filter/cliente', filters).get();
                    },
                    remove: function (id) {
                        return $resource('/api/cliente/:id', { id: id }).remove();
                    },
                    edit: function (cliente) {
                        return $resource('/api/cliente/:id/edit', {id: cliente.id}, {
                            update: {
                                method: 'GET',
                                params: {cliente: cliente}
                            }
                        }).update();
                    },
                    update: function (cliente) {
                        return $resource('/api/cliente/:id', {id: cliente.id}, {
                            update: {
                                method: 'PUT',
                                params: {clienteagente: cliente}
                            }
                        }).update();
                    },
                    create:function (cliente) {
                        return $resource('/api/cliente', null, {
                            update: {
                                method: 'POST',
                                params: {clienteagente: cliente}
                            }
                        }).update();
                    },
                    getSelects:function () {
                        return $resource('/api/get_cliente_select', null, {
                            update: {
                                method: 'POST',
//                                isArray: true
                            }
                        }).update();
                    }
                };
            }]);

angular.module('biosegura')
        .factory('Contactos', ['$resource', function ($resource) {
                return {
                    getAll: function (data) {
                        return $resource('/api/contacto/:data', data).query();
                    },
                    getFiltered: function (filters) {
                        //@holadoc Angular/ngResource  $resource("../rest/api"}).get(); return an object.  $resource("../rest/api").query(); return an array.
                        return $resource('/api/filter/contacto', filters).get();
                    },
                    remove: function (id) {
                        return $resource('/api/contacto/:id', { id: id }).remove();
                    },
                    edit: function (contacto) {
                        return $resource('/api/contacto/:id/edit', {id: contacto.id}, {
                            update: {
                                method: 'GET',
                                params: {contacto: contacto}
                            }
                        }).update();
                    },
                    update: function (contacto) {
                        return $resource('/api/contacto/:id', {id: contacto.id}, {
                            update: {
                                method: 'PUT',
                                params: {contactoagente: contacto}
                            }
                        }).update();
                    },
                    create:function (contacto) {
                        return $resource('/api/contacto', null, {
                            update: {
                                method: 'POST',
                                params: {contactoagente: contacto}
                            }
                        }).update();
                    },
                    getSelects:function () {
                        return $resource('/api/get_contacto_select', null, {
                            update: {
                                method: 'POST',
//                                isArray: true
                            }
                        }).update();
                    }
                };
            }]);



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

angular.module('biosegura')
        .factory('User', ['$http', '$location', '$cookies', function ($http, $location, $cookies) {
                return {
                    doLogin: function (data) {
                        return $http({
                            headers: {'Content-Type': 'application/json'},
                            url: baseUrl + 'api/auth',
                            method: "POST",
                            data: {email: data.email, password: data.password}
                        }).success(function (response) {
                            $cookies.put('auth', JSON.stringify(response));
                        });
                    },

                    doUserLogout: function () {
                        $http({
                            headers: {'Content-Type': 'application/json'},
                            url: baseUrl + 'api/logout',
                            method: "POST"
                        }).success(function () {
                            $cookies.remove('auth');
                            $location.path('/');
                        });
                    },
                    
                    getAuthStatus: function () {
                        return $cookies.get('auth');
                    },
                    
                    getUserObject: function () {
                        return angular.fromJson($cookies.get('auth'));
                    },
                    
                    isAdmin: function () {
                        return this.getAuthStatus() && this.getUserObject().role === 'Administrador';
                    },
                    
                    isCoord: function () {
                        return this.getAuthStatus() && this.getUserObject().role === 'Coordinador';
                    },
                    
                    isConta: function () {
                        return this.getAuthStatus() && this.getUserObject().role === 'Contable';
                    }
                };
            }]);

angular.module('biosegura')
        .factory('Usuarios', ['$resource', function ($resource) {
                return {
                    getAll: function (data) {
                        return $resource('/api/user/:data', data).query();
                    },
                    getFiltered: function (filters) {
                        return $resource('/api/user_filter', filters).get();
                    },
                    remove: function (id) {
                        return $resource('/api/user/:id', { id: id }).remove();
                    },
                    edit: function (user) {
                        return $resource('/api/user/:id/edit', {id: user.id}, {
                            update: {
                                method: 'GET',
                                params: {user: user}
                            }
                        }).update();
                    },
                    update: function (user) {
                        return $resource('/api/user/:id', {id: user.id}, {
                            update: {
                                method: 'PUT',
                                params: {user: user}
                            }
                        }).update();
                    },
                    create:function (user) {
                        return $resource('/api/user', null, {
                            update: {
                                method: 'POST',
                                params: {user: user}
                            }
                        }).update();
                    }
                };
            }]);
 

//# sourceMappingURL=all.js.map
