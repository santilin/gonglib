angular.module("biosegura")
    .directive("hmLeftNav", function() {
        return {
            restrict: "E",
            controller: "navCtrl",
            templateUrl: "ng-templates/nav/nav-left.html"
        };
    });
