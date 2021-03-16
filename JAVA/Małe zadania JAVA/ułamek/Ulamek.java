public class Ulamek{

	private int l; //licznik
	
	private int m; //mianownik
	
	private int znak; // 1 jeśli dodatni, -1 jeśli ujemny

	public Ulamek(int l, int m){

		if(m == 0){
			System.out.println("Mianownik nie może być zerem.");
			System.exit(0);
		}
		if((l > 0 && m > 0 ) || (l < 0 && m < 0))
			znak = 1;
		else
			znak = -1;

		l = mod(l);
		m = mod(m);

		int nwd = nwd(l, m);

		this.l = l / nwd;
		this.m = m / nwd;
		
	}

	public int compareTo(Ulamek arg){

		if(this.znak < arg.znak)
			return -1;
		else if(this.znak > arg.znak)
			return 1;

		// w tym momencie (this) i arg są tego samego znaku;
		if(this.m == arg.m){

			if ( (this.l < arg.l && this.znak > 0) || (this.l > arg.l && this.znak < 0) )
				return -1;

			else if ( (this.l < arg.l && this.znak < 0) || (this.l > 0 && this.znak > 0) )
				return 1;
			else
				return 0;
		}
		else if ( (this.m < arg.m && this.znak > 0) || (this.m > arg.m && this.znak < 0) )
			return 1;
		return -1;
	}

	Ulamek add(Ulamek arg){

		int newM = this.m * arg.m;
		int l1 = this.l * arg.m;
		int l2 = arg.l * this.m;
		int newL;

		if(this.znak != arg.znak){
			if(this.znak < arg.znak)
				newL = l2 - l1;
			else
				newL = l1 - l2;
		}
		else
			newL = this.znak*(l1 + l2);

		return new Ulamek(newL, newM);
	}

 	Ulamek mul(Ulamek arg){
 		int newL = this.l * arg.l;
 		int newM = this.m * arg.m;
 		newL *= this.znak * arg.znak;
 		return new Ulamek(newL, newM);
 	}

	Ulamek sub(Ulamek arg){
		Ulamek pom = new Ulamek((-1) * arg.znak * arg.l, arg.m);
		return this.add(pom);
	}
	Ulamek div(Ulamek arg){
		if(arg.l == 0){
			System.out.println("Nie można dzielić przez 0.");
			System.exit(0);
		}
		Ulamek pom = new Ulamek(arg.znak * arg.m, arg.l);
		return this.mul(pom);
	}

	boolean equals(Ulamek arg){
		if(this.compareTo(arg) == 0)
			return true;
		return false;
	}


	public String toString(){
		if(l == 0)
			return l+"";
		if(m == 1){
			if(this.znak < 0)
				return "-"+this.l;
			return this.l+"";
		}
		else{
			if(this.znak < 0)
				return "-"+l+"/"+m;
			return l+"/"+m;
		}
	}


	private static int mod(int a){
		if (a < 0)
			return a*(-1);
		return a;
	}

	private static int nwd(int x, int y) {
		int temp;
		while (y != 0) {
			temp = y;
			y = x % y;
			x = temp;
		}
		return x;
	}

}