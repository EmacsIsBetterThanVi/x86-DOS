int memcmp(const void * str1, const void * str2, int length){
	for(int i=0; i<n; i++){
		if(str1[i] != str2[i]) return str1[i] - str2[i];
	}
	return 0;
}