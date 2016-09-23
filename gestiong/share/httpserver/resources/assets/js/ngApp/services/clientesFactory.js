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
