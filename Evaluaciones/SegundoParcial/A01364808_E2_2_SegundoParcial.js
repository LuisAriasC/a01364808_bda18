//Luis Carlos Arias Camacho | A01364808 | Segunda Evaluación Parcial 

//Pregunta 1
db.grades.aggregate({$group: {_id: '$student_id'}},{$group: {_id: 1,count: {$sum: 1}}})
db.grades.aggregate([ {$group: {_id: "$student_id"}}]).toArray().length


//Pregunta 2
db.grades.aggregate({$group: {_id: '$class_id'}},{$group: {_id: 1,count: {$sum: 1}}})
db.grades.aggregate([ {$group: {_id: "$class_id"}}]).toArray().length


//Pregunta 3
var mapCalif = function(){this.scores.forEach((value)=>{emit({student:this.student_id,class_id:this.class_id}, value.score)});};
var reduceCalif = function(name, value){var n = Array.avg(value); return n;};
db.grades.mapReduce(mapCalif,reduceCalif,{out:"resCalif"})
db.resCalif.find()


//Pregunta 4
var mapCalif = function(){this.scores.forEach((value)=>{emit({student:this.student_id,class_id:this.class_id}, value.score)});};
var reduceCalif = function(name, value){var n = Array.avg(value); return n;};
db.grades.mapReduce(mapCalif,reduceCalif,{out:"resCalif"})
db.resCalif.find()


//Pregunta 5
db.resCalif.find().sort({value: 0}).limit(1)


//Pregunta 6
db.grades.aggregate([{ $group: { _id: "$student_id", total: { $sum: 1 } } },{ $sort: { total: -1 }}, {$limit: 1}]);


//Pregunta 7
var mapCurso = function(){this.scores.forEach((value)=>{emit(this.class_id, value.score)});};
var reduceCurso = function(name, value){var n = Array.avg(value); return n;};
db.grades.mapReduce(mapCurso,reduceCurso,{out:"resCursos"})
db.resCursos.find().sort({value: 1}).limit(1)


//EXTRA... PROMEDIO POR ALUMNO
var mapPromEstudiante = function(){this.scores.forEach((value)=>{emit(this.student_id, value.score)});};
var reduceCalifs = function(name, value){var n = Array.avg(value); return n;};
db.grades.mapReduce(mapPromEstudiante,reduceCalifs,{out:"resPromedioGeneral"})
db.resPromedioGeneral.find()
