package main

// Just to demonstrate a function 
func getsize() int {
	return 5
}

func main() {
	var pyramid_size int = getsize()
	for i := 0; i < pyramid_size; i++ {
		for j := 0; j < pyramid_size - i -1; j++ {
			print(" ")
		}
		for j := 0; j < i*2+1; j++ {
			print("X")
		}
		print("\n");
	}
}
