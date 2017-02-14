describe('tester2 - pointers', ()=>{
	before((next)=>{
		try{
			testers.tester2.getString.here;
			testers.tester2.getPointer.here;
			testers.tester2.getValue.here;
			testers.tester2.getValueFromPointer.here;
		}catch(e){
			return next(e);
		}
		next();
	});

	describe('simple pointer test:', ()=>{
		const val = testers.tester2.getValue.bind(testers.tester2),
			  point = testers.tester2.getPointer.bind(testers.tester2),
			  valFromP = testers.tester2.getValueFromPointer.bind(testers.tester2);
		it('should be the same value', (done)=>{
			expect( valFromP(point())  ).to.be.equal(val());
			done();
		});
	});

	describe('char pointer test:', ()=>{
		const fn = testers.tester2.getString.bind(testers.tester2);
		it('should return a string and NOT a pointer value', (done)=>{
			expect( fn()  ).to.be.equal("this is a string");
			done();
		});
	});
});