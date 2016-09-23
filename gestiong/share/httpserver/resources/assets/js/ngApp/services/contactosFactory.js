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
