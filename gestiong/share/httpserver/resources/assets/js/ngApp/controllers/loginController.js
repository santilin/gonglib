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

