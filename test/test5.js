describe('tester5 - setter & getter', ()=>{
	let tester ,get;
	const lorem = 'lorem ipsum dolor sit amet ac in risus ligula nisl rutrum odio et mattis et lorem ut morbi';
	before((next)=>{
		try{
			tester = testers.tester5.tester;
			get = testers.tester5.get;
			tester.here;
			get.here;
		}catch(e){
			return next(e);
		}
		next();
	});
	it('Should return a simple object with 2 keys { i,str }',(done)=>{ 
		const obj = get();
		expect(obj).to.eql({ 
			i: 777,
			str: lorem
		});
		done();
	});
	// describe('simple function pointer void(*)():',()=>{
	// 	it('Should return a simple object with 2 keys',(done)=>{ 
	// 	});
	// });
});