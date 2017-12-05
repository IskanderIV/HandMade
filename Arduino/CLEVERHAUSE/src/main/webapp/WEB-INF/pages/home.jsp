<%@ page contentType="text/html; charset=UTF-8" language="java"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<c:set var="contextPath" value="${pageContext.request.contextPath}"/>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Cleverhause.ru, welcome page!</title>
    <!-- Bootstrap -->
    <link href="${contextPath}/resources/css/bootstrap.min.css" rel="stylesheet">
    <link href="${contextPath}/resources/styles/jumbotron-narrow.css" rel="stylesheet">
</head>
<body>
<div class="container">
    <div class="header">
        <ul class="nav nav-pills pull-right">
            <li class="active"><a href="#">Home</a></li>
            <li><a href="#">MyDevices</a></li>
            <li><a href="#">About</a></li>
            <li><a href="#">Contact</a></li>
        </ul>
        <h3 class="text-muted">My name</h3>
    </div>

    <div class="jumbotron">
        <h2>Welcome to the cleverhause.ru</h2>
        <img src="${contextPath}/resources/img/chetyrehlistnij_klever-300x274.png"/>
        <p class="lead"></p>
        <p><a class="btn btn-lg btn-success" href="${contextPath}/login" role="button">Sign in</a></p>
    </div>

    <div class="row marketing">
        <div class="col-lg-6">
        </div>
        <div class="col-lg-6">
        </div>
    </div>

    <div class="footer">
        <p>&copy; Ivanov's 2017</p>
    </div>

</div> <!-- /container -->

<!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
<!--<script src="/resources/jQuery/jquery-3.2.1.min.js"></script>-->
<script src="${contextPath}/resources/jQuery/jquery-3.2.1.min.js"></script>
<!-- Include all compiled plugins (below), or include individual files as needed -->
<script src="${contextPath}/resources/js/bootstrap.min.js"></script>
</body>
</html>
