/*
 * MD5c.h
 *
 * Modified interface to generic MD5 digest code 
 */

typedef char Md5RawDigest[16];
typedef char Md5CodedDigest[33];
typedef void *Md5Context;

extern void
MD5DigestThreeStrings(const char *string1, 
                      const char *string2, 
                      const char *string3, 
                      Md5RawDigest digest);
extern void
MD5DigestTwoStrings(const char *string1, 
                    const char *string2, 
                    Md5RawDigest digest);
extern void
MD5DigestString(const char *string, Md5RawDigest digest);

extern void
MD5EncodeDigest(const Md5RawDigest raw, Md5CodedDigest coded);


extern Md5Context
MD5Begin(void);

extern void
MD5DigestBytes(Md5Context ctx, unsigned char *bytes, int count);

extern void
MD5DigestLong(Md5Context ctx, unsigned long l);

extern void 
MD5End(Md5Context ctx, Md5RawDigest raw);

