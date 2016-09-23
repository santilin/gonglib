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
