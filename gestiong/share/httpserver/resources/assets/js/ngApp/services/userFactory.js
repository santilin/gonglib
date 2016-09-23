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
