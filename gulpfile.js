const 
	fs = require('fs'),
	gulp = require('gulp'),
  del = require('del'),
  spawn = require("child_process").spawnSync;


gulp.task('clean-build',function(){
  return del.sync('./bintest/**/*.node');
});

gulp.task('build-test',['clean-build' ], function (){
  return spawn ("npm", ["run" , "test:build"], {
    stdio: "inherit" 
    });
});

gulp.task('test',['build-test' ], function (){
  spawn ("npm", ["run" , "test:tmp"], {
    stdio: "inherit" 
    });
});

gulp.task('init',[], function (){
  spawn ("npm", ["run" , "init:build"], {
    stdio: "inherit" 
    });
});


gulp.task('watch',function(){
 return gulp.watch(['src/**/*.h' , 'test/**/*.js' , 'bintest/**/*.cc'] , ['test' ]);
});


let defaultTasks =['test', 'watch' ];
try{
  fs.lstatSync('./bintest/build');
 }catch(e)  {
    defaultTasks.unshift('init');
}
gulp.task( 'default',defaultTasks);

