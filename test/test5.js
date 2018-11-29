
describe('tester5 - setter & getter', ()=>{
	let tester ,get;
	const worngObjects = [
		null, //not obj
		{},// empty
		{i :66 },// missing str
		{str :lorem },// missing str
		{i:lorem, str :lorem },// worng values
		{i:777, str :777 },// worng values
		{i:lorem, str :777 }// worng values
	 ];
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
	describe('simple function binding  boolean(struct1 ,struct2) <->  struct1 == struct2 :',()=>{
		it('Should throw an error with the worng values',(done)=>{
			const fatalCheck = (i,j)=>{
					try{
						tester(i,j);
						done("should have throwed an Error but didn't!");
					} catch(e) {
					/*OK!*/
					}
				},
				o = get();
		 	worngObjects.forEach( i =>  fatalCheck(i ,o) );
		 	worngObjects.forEach( i =>  fatalCheck(o ,i) );
			worngObjects.forEach( i =>  worngObjects.forEach( j =>  fatalCheck(j ,i) ) );
			done();
		});
		it('Behave like a normal compare function',(done)=>{
			const o = get(),
				 i = { i : 777 , str:lorem }
				 test =(flag) => expect( tester(o,i) ).to.be.equal(flag);
				 test(true);
				 i.j =55;
				 test(true);
				 i.i =8;
				 test(false);
				 o.i =  i.i;
				 test(true);
				 o.yyy = [{} ,"some text"];
				 test(true);
				 i.str = "anther random text";
				 test(false);
				 o.str = i.str;
				 test(true);
				 done();
		});		
	});
});