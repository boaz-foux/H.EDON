describe('tester3 - function callbacks', ()=>{
		before((next)=>{
		try{
			testers.tester3.callbackBinding.here;
		}catch(e){
			return next(e);
		}
		next();
	});

	describe('simple pointer test:', ()=>{
		const fn = testers.tester3.callbackBinding.bind(testers.tester3),
			callback  = function(a,b){
				return a * b;// a + b;
			};
			
		it('should be the same value', (done)=>{
			var i =8, j =9;
			expect( fn(i,j,callback)  ).to.be.equal(i*j);
			done();
		});
	});

});