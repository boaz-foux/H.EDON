describe('tester3 - function callbacks', ()=>{
		const n = 2;//fixed(random(5,2),0),
			 numbers = randoms(4,100,-100,n),
			 adjust = (num)=> fixed(num,n),
			 functions = [
			 (i,j)=> i+j,
			 (i,j)=> i+j+j,
			 (i,j)=> i+i+j,
			 (i,j)=> i*i,
			 (i,j)=> j*j,
			 (i,j)=> j*j+i,
			 (i,j)=> n,
			 (i,j)=> i+j+(i*j) +n
			 ];
		let testCases ={};

	before((next)=>{
		try{
			testCases = cases(functions,numbers,numbers);
			testers.tester3.callback.here;
			testers.tester3.recallback.here;
			testers.tester3.callback0.here;
			testers.tester3.callback1.here;
			testers.tester3.callback2.here;
			testers.tester3.callback3.here;
			testers.tester3.callbackChar.here;

		}catch(e){
			return next(e);
		}
		next();
	});

	describe('simple function pointer void(*)():',()=>{
		const fn = testers.tester3.callback.bind(testers.tester3),
			  rfn = testers.tester3.recallback;
		let flag = 0;

		
		it('should not do anything when recalling with null pointer',(done)=>{
			expect(rfn).withArgs().to.not.throwError();
			done();
		});
		describe('recall check with a void(*)() pointer:',()=>{
		before((next)=>{
			try{
				fn(()=>{
					flag++;
				});
			}catch(e){
				return next(e);
			}
			next();
		});
			it('should be fine with a simple void() function call',(done)=>{
				expect(flag).to.be.equal(1);
				done();
			});

			it('should call void(*)() 3 more times when recalling with a real pointer',(done)=>{
				expect(rfn).withArgs().not.to.throwError();
				expect(flag).to.be.equal(4);
				done();
			});
		});
	}); 

	describe('simple function pointer int(*)()',()=>{
		const fn = testers.tester3.callback0.bind(testers.tester3);
		it('should return a correct value of int',(done)=>{
			numbers
			.forEach( i => {
				let value = toInt(random(1000,-1000,n));
					expect(fn(()=> value )).to.be.equal( value )
				});
			done();
		}); 
	}); 

	describe('function pointer., i ,f(p)(i) -> p(i)',()=>{
		const fn = testers.tester3.callback1.bind(testers.tester3);
		it('should return the correct value of float with diffrent int input',
			(done)=>{
				numbers
				.forEach( i => {
					let number = random(1,0,n);
					let value = adjust(fn( toInt(i) , j => j +number ));
					expect(value).to.be.equal( toInt(i) + number)
				});
			done();
		});
		it('should return the correct value of float without the other int input',
			(done)=>{
				numbers.forEach((i)=>{
					numbers.forEach( j => {
						let number = adjust(fn( i ,()=> j ));
						expect(number).to.be.equal(j);
					});
				});
			done();
		}); 
	});
	describe('function pointer., d(p)(i,d),i,d  -> p(i,d)',()=>{
		const fn = testers.tester3.callback2.bind(testers.tester3);

		it('should return the correct value of float with diffrent inputs',
			(done)=>{
				testCases.forEach( array => {
					let [pfn,i,d] = array;
					i = toInt(i);
					let v = fn(pfn,i,d);
					expect( v ).to.be.equal( pfn(i,d) );
				});
				done();
			});
	});
	describe('function pointer., f, f(p)(i,d),i,d) -> f * p(i,d)',()=>{
		const fn = testers.tester3.callback3.bind(testers.tester3);
		it('should return the correct value of float with diffrent inputs',
			(done)=>{
				numbers.forEach((f)=>{
					testCases.forEach( array => {
						let [pfn,i,d] = array;
						i = toInt(i);
						let v = fn(f,pfn,i,d);
						expect(v).to.be.equal( f*pfn(i,d) );
					});
				});
				done();
			});	
	});
	describe('function pointer., i ,char *(p)(i) -> p(i)',()=>{
		const fn = testers.tester3.callbackChar.bind(testers.tester3),
		messages = strings(50);
		it( 'should return diffrent messages with diffrent index' ,(done)=>{ 
			messages.forEach((data,index)=>{
				expect( fn(index,(i) => messages[i]) ).to.be.equal( data );
			});
			done(); 
		});
	});
});


