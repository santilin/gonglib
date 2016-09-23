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
 
