package main

import (
	"errors"
	"log"
	"net/http"
)

func trace(w http.ResponseWriter, r *http.Request) {
	log.Println("called function")
}

func main() {
	http.HandleFunc("/trace", trace)
	err := http.ListenAndServe(":5173", nil)
	if errors.Is(err, http.ErrServerClosed) {
		log.Fatal("server closed\n")
	} else if err != nil {
		log.Fatalf("error starting server: %s\n", err)
	}
	log.Println("server started. listening on port 5173")
}
