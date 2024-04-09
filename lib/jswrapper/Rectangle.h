#ifndef _RECTANGLE_
#define _RECTANGLE_
#define DEBUG(Argv)
// create a process object, which keep argc, argvList
class Rectangle
{
   private:
        int length;
        int width;
        int *buf=NULL;
   public:
        Rectangle(int length,int width);
        ~Rectangle();
        int getSize();
        int getLength();
        int getWidth();
        int getBuf(int* buf,int len); // TODO try for typedarray
        int *getBufPtr();
        int setBuf(int *buf,int len);
};
#define BUFFSIZE 10
Rectangle::Rectangle(int length,int width)
{
    this->length = length;
    this->width = width;
    this->buf = (int*)calloc(BUFFSIZE,sizeof(int));
}
Rectangle::~Rectangle()
{
    if (this->buf!=NULL) {
        free(this->buf);
    }
}
int Rectangle::getSize()
{
    return (this->length * this->width);
}

int Rectangle::getLength()
{
    return this->length;
}

int Rectangle::getWidth()
{
    return this->width;
}
int Rectangle::getBuf(int *inbuf,int len)
{
    int actual = (len>BUFFSIZE) ? BUFFSIZE:len;
    memcpy(inbuf,this->buf,actual*sizeof(int));
    return actual;
}
int *Rectangle::getBufPtr()
{
    return this->buf;
}
int Rectangle::setBuf(int *outbuf,int len)
{
    int actual = (len>BUFFSIZE) ? BUFFSIZE:len;
    memcpy(this->buf,outbuf,actual*sizeof(int));
    return actual;
}
#endif //_RECTANGLE_