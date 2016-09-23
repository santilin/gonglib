angular.module('biosegura')
        .controller('mainCtrl', ['$scope', function ($scope) {
                $scope.AppTitle = "Biosegura";
                $scope.volverAtras = function () {
                    window.history.back();
                };
            }]); 
