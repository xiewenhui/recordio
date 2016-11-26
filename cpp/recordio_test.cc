#include "recordio.h"

#include <string>
#include <boost/scoped_ptr.hpp>

#include "file.h"
#include "gtest/gtest.h"

#include "document.pb.h"

using namespace commonlib::util;

class RecordIOTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        r1.set_docid(10);
        r1.mutable_links()->add_forward(20);
        r1.mutable_links()->add_forward(40);
        r1.mutable_links()->add_forward(60);
        r1.add_name();
        r1.mutable_name(0)->add_language();
        r1.mutable_name(0)->mutable_language(0)->set_code("en-us");
        r1.mutable_name(0)->mutable_language(0)->set_country("us");
        r1.mutable_name(0)->add_language();
        r1.mutable_name(0)->mutable_language(1)->set_code("en");
        r1.mutable_name(0)->set_url("http://A");
        r1.add_name();
        r1.mutable_name(1)->set_url("http://www.weibo.com/zhihu?from=feed&loc=nickname");
        r1.add_name();
        r1.mutable_name(2)->add_language();
        r1.mutable_name(2)->mutable_language(0)->set_code("en-gb");
        r1.mutable_name(2)->mutable_language(0)->set_country("gb");

        r2.set_docid(20);
        r2.mutable_links()->add_backward(10);
        r2.mutable_links()->add_backward(30);
        r2.mutable_links()->add_forward(80);
        r2.add_name();
        r2.mutable_name(0)->set_url("http://C");

        r3.set_docid(30);
        r3.mutable_links()->add_backward(100);
        r3.add_name();
        r3.mutable_name(0)->set_url("http://D");

        ::commonlib::util::file::ReadFileToString("./file.h", r3.mutable_payload());
    }
    virtual void TearDown() {
        m_reader.reset();
        m_writer.reset();
    }

protected:
    boost::scoped_ptr<RecordReader> m_reader;
    boost::scoped_ptr<RecordWriter> m_writer;
    Document r1;
    Document r2;
    Document r3;
};

TEST_F(RecordIOTest, TestNextMessageSequnce) {
    File *file = File::Open("./test.dat", "w+");
    ASSERT_TRUE(file->Open());

    m_writer.reset(new RecordWriter(file));

    ASSERT_TRUE(m_writer->WriteProtocolMessage(r1));
    ASSERT_TRUE(m_writer->WriteProtocolMessage(r2));
    ASSERT_TRUE(m_writer->WriteProtocolMessage(r3));

    ASSERT_TRUE(file->Close());
    delete file;

    LOG(INFO) << "END WRITE";
    
    file = File::Open("./test.dat",  "r");
    ASSERT_TRUE(file->Open());

    m_reader.reset(new RecordReader(file));

    Document message;
    ASSERT_TRUE(m_reader->ReadProtocolMessage(&message));
    LOG(INFO) << message.DebugString();
    ASSERT_TRUE(message.docid() == r1.docid());

    ASSERT_TRUE(m_reader->ReadProtocolMessage(&message));
    ASSERT_TRUE(message.docid() == r2.docid());

    ASSERT_TRUE(m_reader->ReadProtocolMessage(&message));
    ASSERT_TRUE(message.docid() == r3.docid());

    LOG(INFO) << message.DebugString();

    LOG(INFO) << "modifytime [" << file->ModifyTime();

    //ASSERT_TRUE(m_reader->ReadProtocolMessage(&message));
    ASSERT_TRUE(file->Close());
}

