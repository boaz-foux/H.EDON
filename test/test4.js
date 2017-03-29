describe('tester4 - statics', ()=>{
	before((next)=>{
		try{
			testers.tester4.staticArray.here;
			testers.tester4.i.here;
		}catch(e){
			return next(e);
		}
		next();
	});
	it('Should return a static int array',(done)=>{ 
		const fn = testers.tester4.staticArray;
		expect(fn()).to.eql([3,4,6,7,8,99,10,676]);
		done(); 
	});
		it('Should return a static int',(done)=>{ 
		const fn = testers.tester4.i;
		expect(fn()).to.be.equal(5558);
		done(); 
	});
});