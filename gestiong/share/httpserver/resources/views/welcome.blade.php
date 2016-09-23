<!DOCTYPE html>
<html>
    <head>
        <!-- Metas -->
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">

        <!--Titulo-->
        <title>Aseaga App</title>

        <!-- Bootstrap -->
        <link href="{{ asset('css/libs/bootstrap.min.css')}}" rel="stylesheet">
        <!-- Font Awesome -->
        <link href="https://maxcdn.bootstrapcdn.com/font-awesome/4.6.3/css/font-awesome.min.css" rel="stylesheet">

        <link rel="stylesheet" href="{{ asset('css/libs/ng-table.min.css')}}">
        <!--Estilos-->
        <link href="{{ asset('css/libs/custom.min.css')}}" rel="stylesheet">
        <link href="{{ asset('css/libs/angular-ui-notification.min.css')}}" rel="stylesheet">
        <link href="{{ asset('css/app.css')}}" rel="stylesheet" type="text/css" >

    </head>


    <body class="nav-sm" ng-app="aseaga" ng-cloak> 
        <div class="container body" ng-controller="mainCtrl">
            <div class="main_container">
                <hm-left-nav></hm-left-nav>
                <hm-top-nav></hm-top-nav>
                <!-- page content -->
                <div class="right_col" role="main">
                    <ng-view ng-cloak></ng-view>
                </div>
                <!-- /page content -->
            </div>
        </div>


        <!-- jQuery -->    
        <script src="{{ asset('js/libs/jquery.min.js')}}"></script>
        <!-- Bootstrap -->
        <script src="{{ asset('js/libs/bootstrap.min.js')}}"></script>
        <!-- NProgress -->
        <script src="{{ asset('js/libs/nprogress.js')}}"></script>
        <!--Angular-->
        <script>var baseUrl = "{{ url('/') }}/";</script>
        <script src="{{ asset('js/libs/angular.min.js')}}"></script>
        <script src="{{ asset('js/libs/angular-route.min.js')}}"></script>
        <script src="{{ asset('js/libs/angular-resource.min.js')}}"></script>
        <script src="{{ asset('js/libs/angular-cookies.min.js')}}"></script>
        <script src="{{ asset('js/libs/ng-table.min.js')}}"></script>
        <script src="{{ asset('js/libs/angular-animate.min.js')}}"></script>
        <script src="{{ asset('js/libs/angular-ui-notification.min.js')}}"></script>
        <!--Scripts-->
        <script src="{{ asset('js/all.js')}}"></script>
        <script src="{{ asset('js/aseaga.min.js')}}"></script>
    </body> 
    <script>
    $(document).ready(function(){
        $('[data-toggle="tooltip"]').tooltip(); 
    });
    </script>
</html>
