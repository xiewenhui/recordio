package recordio

import (
	"fmt"
	"testing"
)

func TestRecordIO(t *testing.T) {
	r1 := &Document{
		Docid:   int64(10),
		Payload: "hello 10",
	}
	r1.Links = new(Links)
	r1.Links.Backward = make([]int64, 0, 10)
	r1.Links.Forward = make([]int64, 0, 10)
	r1.Links.Backward = append(r1.Links.Backward, 10)
	r1.Links.Backward = append(r1.Links.Backward, 13)
	r1.Links.Forward = append(r1.Links.Forward, 15)
	r1.Links.Forward = append(r1.Links.Forward, 17)

	url := "http://www.weibo.com/zhihu?from=feed&loc=nickname"
	r2 := &Document{
		Docid:   int64(20),
		Payload: "hello 20",
	}
	r2.Name = make([]*Name, 0, 10)
	name := new(Name)
	name.Url = url
	r2.Name = append(r2.Name, name)

	r3 := &Document{
		Docid:   int64(30),
		Payload: "hello 30",
	}

	datapath := "/tmp/recordiotest.dat"
	writer, err := NewRecordWriter(datapath)
	if err != nil {
		t.Fatal(err)
	}

	if _, err := writer.WriteProtocolMessage(r1); err != nil {
		t.Fatal(err)
	}
	if _, err := writer.WriteProtocolMessage(r2); err != nil {
		t.Fatal(err)
	}
	if _, err := writer.WriteProtocolMessage(r3); err != nil {
		t.Fatal(err)
	}
	writer.Finish() // close underlying fd

	msg := &Document{}
	reader, err := NewRecordReader(datapath)
	if err != nil {
		t.Fatal(err)
	}

	// Read Record r1
	if _, err := reader.ReadProtocolMessage(msg); err != nil {
		t.Fatal(err)
	}
	if msg.Docid != r1.Docid || msg.Payload != r1.Payload {
		t.Fatalf("data mismatch: %v != %v", msg, r1)
	}
	fmt.Println(msg.Links.Backward) // [10 13]
	fmt.Println(msg.Links.Forward)  // [15 17]

	// Read Record r2
	if _, err := reader.ReadProtocolMessage(msg); err != nil {
		t.Fatal(err)
	}
	if msg.Docid != r2.Docid || msg.Payload != r2.Payload {
		t.Fatalf("data mismatch: %v != %v", msg, r2)
	}
	fmt.Println(len(msg.Name))
	fmt.Println(msg.Name[0].Url)
	if msg.Name[0].Url != url {
		t.Fatalf("data mismatch: %v != %v", msg, r2)
	}

	// Read Record r3
	if _, err := reader.ReadProtocolMessage(msg); err != nil {
		t.Fatal(err)
	}
	if msg.Docid != r3.Docid || msg.Payload != r3.Payload {
		t.Fatalf("data mismatch: %v != %v", msg, r3)
	}

	if _, err := reader.ReadProtocolMessage(msg); err != nil {
		fmt.Println("ok : here is a expected err ", err)
	}
	reader.Finish() // close underlying fd

}
