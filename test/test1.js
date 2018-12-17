describe('tester1 - numbers', ()=>{
	before((next)=>{
		try{
			testers.tester1.test0.here;
			testers.tester1.test1.here;
			testers.tester1.test2.here;
			testers.tester1.test3.here;
			testers.tester1.test10.here;
		}catch(e){
			return next(e);
		}
		next();
	});

	describe('test0 - double()', ()=>{
		const fn = testers.tester1.test0.bind(testers.tester1);
		it('should return the value of 4.891', (done)=>{
			expect(fn()).to.be.equal(4.891);
			done();
		});
		checkup.length(fn,0);
	});

	describe('test1 - int(double j)', ()=>{
		const fn = testers.tester1.test1.bind(testers.tester1);
		it('should return the floor value of 0.2, 0', (done)=>{
			expect(fn(0.2)).to.be.equal(0);
			done();
		});
		it('should return the floor value of 1.1, 1', (done)=>{
			expect(fn(1.1)).to.be.equal(1);
			done();
		});
		it('should return the same value when double is a whole number', (done)=>{
			const i = Math.ceil( Math.random()*987 );
			expect(fn(i)).to.be.equal(i);
			done();
		});
		checkup.length(fn,1);
	});

	describe('test2 - float(double i , double j)', ()=>{
		const fn = testers.tester1.test2.bind(testers.tester1);
		it('should not when multiplaing small numbers , should return a small float', (done)=>{
			const epsilon = 0.000000001;
			expect(fn(epsilon,epsilon)).to.not.be.equal(0);
			done();
		});
		checkup.length(fn,2);
	});

	describe('test3 - double(float i , double j, int k)', ()=>{
		const fn = testers.tester1.test3.bind(testers.tester1);
		it('should return the correct value of 4.25', (done)=>{
			expect(fn(0.5,0.5,4)).to.be.equal(4.25);
			done();
		});
		checkup.length(fn,3);
	});


	describe('test10 - int(i,d,f,i,d,f,i,d,f,d)', ()=>{
		const fn = testers.tester1.test10.bind(testers.tester1);
		it('should return the correct value of 36', (done)=>{
			expect(fn(1,0.5,4,1,1,1,5,5,0.2,5)).to.be.equal(36);
			done();
		});
		checkup.length(fn,10);
	});

});