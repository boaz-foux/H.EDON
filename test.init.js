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