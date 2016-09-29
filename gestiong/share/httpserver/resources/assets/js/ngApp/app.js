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
