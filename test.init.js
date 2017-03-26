let testers = global.testers = {};
const 
	// bindings = require('bindings'),
	fs = require('fs'), 
	targets = JSON.parse(fs.readFileSync('./bintest/binding.gyp').toString()).targets,
	tragetsObject = targets.reduce((rtn,{target_name})=>{
		rtn[target_name] = require('./bintest/build/Release/' +target_name+ '.node');// bindings('target_name');
		return rtn;
	},testers);
global.expect = require('expect.js');

global.checkup = {
	length: (fn,length) => {
		if(length > 0){
			it('should throw exeption when calling with less arguments',
				(done)=>{
				expect(fn).withArgs( ...Array(length -1).fill(777) ).to.throwError();
					done();
				});
		}
		it('should throw exeption when calling with to many arguments',
				(done)=>{
				expect(fn).withArgs( ...Array(length + 1).fill(777) ).to.throwError();
					done();
				});
	}
};

const 
	fixed = global.fixed = (n,fix)=>{
		return Number(n.toFixed(fixed));
	},
	random  = global.random =(to,from,_fixed)=>{
		from = from || 0;
		to = to || 10000;
		let n = Math.random()*(to-from) + from;
		if(_fixed){
			n = fixed(n,_fixed);
		}
		return n;
	},
	randoms = global.randoms =(n,to,from,fixed)=>{
		let array =[];
		for(let i = 0 ; i < n ; i++){
			array.push( random(to,from,fixed) );
		}
		return array;
	},
	toInt = global.toInt = i => Math.floor( Number(i).toFixed(0)),
	randomString = global.string = ()=> Date.now().toString(  toInt( random(36,16) ) ),
	randomStrings = global.strings = (n) => [...Array(n)].map(randomString),
	cases = global.cases =  function(){
		const arr = [...arguments];
		switch(true){
			case arr.length ==0:
				return [];
			case arr.length == 1:
			return arr[0];
			default:
		}
	    let result = [];
	    let rest = cases(arr.slice(1));  
	    for (let i = 0; i < rest.length; i++) {
	      for (let j = 0; j < arr[0].length; j++) {
	        result.push( [arr[0][j] , ...rest[i] ]);
	      }
	    }
	    return result;
	};