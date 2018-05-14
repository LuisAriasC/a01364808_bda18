import { read } from 'fs';

var express = require('express');
var app = express();

var neo4j = require('node-neo4j');
var db = new neo4j('http://neo4j:test@neo4j:7474');

app.use('/', express.static(__dirname + '/view'));

app.get('/tools/load', function (req, res, next) {
    db.insertNode({
        name: 'Darth Vader #' + parseInt(Math.random() * 100),
        sex: 'male'
    }, ['Person'], function (err, node) {
        if (err) return next(err);

        res.json(node);
    });
});

app.get('/tools/create', function(req, res, next){
    db.insertNode({
        name: "Recetas"
    }, ['Almacen'], function (err, node){
        if (err) return next(err);
        res.json(node);
    });
    db.insertNode({
        name: "Materia Prima"
    }, ['Almacen'], function (err, node){
        if (err) return next(err);
        res.json(node);
    });
    db.insertNode({
        name: "Productos"
    }, ['Almacen'], function (err, node){
        if (err) return next(err);
        res.json(node);
    });
});


app.get('/tools/drop', function (req, res, next) {
    db.cypherQuery("MATCH (n) DETACH DELETE n", function (err, result) {
        if (err) return next(err);
        res.json(result);
    });
});

app.get('/persons', function (req, res, next) {
    db.cypherQuery("MATCH (person:Person) RETURN person", function (err, result) {
        if (err) return next(err);
        res.json(result.data);
    });
});

app.get('/almacenes', function (req, res, next) {
    db.cypherQuery("MATCH (almacen:Almacen) RETURN almacen", function (err, result) {
        if (err) return next(err);
        res.json(result.data);
    });
});


app.listen(3000, function () {
    console.log('started');
});