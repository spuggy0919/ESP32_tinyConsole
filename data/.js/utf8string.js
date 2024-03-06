/**
 *
 * last two functions is not used, will be remove later
 * they copy from website no copyright notice, if any problem, pls inform me to remove
 */
var convertUtf8 = (function() {
    /* 
        //usage:
        var inputString = "Hello, World!";
        var resultBytes = stringToBytes(inputString);
        console.log(resultBytes);
    */
    function stringToBytes(inputString) {
        var byteArray = [];
        for (var i = 0; i < inputString.length; i++) {
            byteArray.push(inputString.charCodeAt(i) & 0xFF); // Keep only the least significant byte
        }
        return byteArray;
    }
    /*
    // Example usage:
    var utf8Bytes = [72, 101, 108, 108, 111, 44, 32, 87, 111, 114, 108, 100, 33];
    var utf16String = utf8BytesToUtf16String(utf8Bytes);
    console.log(utf16String);
    */
    function utf8BytesToUtf16String(utf8Bytes) {
        var utf16String = "";
        var i = 0;
    
        while (i < utf8Bytes.length) {
            var byte1 = utf8Bytes[i++];
            var byte2, byte3, byte4;
            var unicode;
    
            if ((byte1 & 0x80) === 0) {
                // 1-byte character (ASCII)
                unicode = byte1;
            } else if ((byte1 & 0xE0) === 0xC0) {
                // 2-byte character
                byte2 = utf8Bytes[i++];
                unicode = ((byte1 & 0x1F) << 6) | (byte2 & 0x3F);
            } else if ((byte1 & 0xF0) === 0xE0) {
                // 3-byte character
                byte2 = utf8Bytes[i++];
                byte3 = utf8Bytes[i++];
                unicode = ((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F);
            } else if ((byte1 & 0xF8) === 0xF0) {
                // 4-byte character
                byte2 = utf8Bytes[i++];
                byte3 = utf8Bytes[i++];
                byte4 = utf8Bytes[i++];
                unicode = ((byte1 & 0x07) << 18) | ((byte2 & 0x3F) << 12) | ((byte3 & 0x3F) << 6) | (byte4 & 0x3F);
            } else {
                // Invalid UTF-8 sequence
                throw new Error('Invalid UTF-8 sequence detected.');
            }
    
            // Convert Unicode to UTF-16 and append to the result string
            utf16String += String.fromCharCode(unicode);
        }
    
        return utf16String;
    }
    

    /*
     * unicode string to utf-8
     * @param text 字符串
      @returns {} utf-8编码
     */
    function toURIBytes(text) {
        var result = [], i = 0;
        text = encodeURI(text);
        while (i < text.length) {
            var c = text.charCodeAt(i++);
            // if it is a % sign, encode the following 2 bytes as a hex value
            if (c === 37) {
                result.push(parseInt(text.substr(i, 2), 16))
                i += 2;
                // otherwise, just the actual byte
            } else {
                result.push(c)
            }
        }
        return coerceArray(result);
    }
    /**
     * utf8 byte to unicode string
     * @param utf8Bytes
     * @returns {string}
     */
    function utf8ByteToUnicodeStr(utf8Bytes){
        var unicodeStr ="";
        for (var pos = 0; pos < utf8Bytes.length;){
            var flag= utf8Bytes[pos];
            var unicode = 0 ;
            if ((flag >>>7) === 0 ) {
                unicodeStr+= String.fromCharCode(utf8Bytes[pos]);
                pos += 1;
            } else if ((flag &0xFC) === 0xFC ){
                unicode = (utf8Bytes[pos] & 0x3) << 30;
                unicode |= (utf8Bytes[pos+1] & 0x3F) << 24;
                unicode |= (utf8Bytes[pos+2] & 0x3F) << 18;
                unicode |= (utf8Bytes[pos+3] & 0x3F) << 12;
                unicode |= (utf8Bytes[pos+4] & 0x3F) << 6;
                unicode |= (utf8Bytes[pos+5] & 0x3F);
                unicodeStr+= String.fromCodePoint(unicode) ;
                pos += 6;
            }else if ((flag &0xF8) === 0xF8 ){
                unicode = (utf8Bytes[pos] & 0x7) << 24;
                unicode |= (utf8Bytes[pos+1] & 0x3F) << 18;
                unicode |= (utf8Bytes[pos+2] & 0x3F) << 12;
                unicode |= (utf8Bytes[pos+3] & 0x3F) << 6;
                unicode |= (utf8Bytes[pos+4] & 0x3F);
                unicodeStr+= String.fromCodePoint(unicode) ;
                pos += 5;
            } else if ((flag &0xF0) === 0xF0 ){
                unicode = (utf8Bytes[pos] & 0xF) << 18;
                unicode |= (utf8Bytes[pos+1] & 0x3F) << 12;
                unicode |= (utf8Bytes[pos+2] & 0x3F) << 6;
                unicode |= (utf8Bytes[pos+3] & 0x3F);
                unicodeStr+= String.fromCodePoint(unicode) ;
                pos += 4;
            } else if ((flag &0xE0) === 0xE0 ){
                unicode = (utf8Bytes[pos] & 0x1F) << 12;;
                unicode |= (utf8Bytes[pos+1] & 0x3F) << 6;
                unicode |= (utf8Bytes[pos+2] & 0x3F);
                unicodeStr+= String.fromCharCode(unicode) ;
                pos += 3;
            } else if ((flag &0xC0) === 0xC0 ){ //110
                unicode = (utf8Bytes[pos] & 0x3F) << 6;
                unicode |= (utf8Bytes[pos+1] & 0x3F);
                unicodeStr+= String.fromCharCode(unicode) ;
                pos += 2;
            } else{
                unicodeStr+= String.fromCharCode(utf8Bytes[pos]);
                pos += 1;
            }
        }
        return unicodeStr;
    }
    function checkInt(value) {
        return (parseInt(value) === value);
    }
    function checkInts(arrayish) {
        if (!checkInt(arrayish.length)) { return false; }
        for (var i = 0; i < arrayish.length; i++) {
            if (!checkInt(arrayish[i]) || arrayish[i] < 0 || arrayish[i] > 255) {
                return false;
            }
        }
        return true;
    }
    function coerceArray(arg, copy) {
        // ArrayBuffer view
        if (arg.buffer && arg.name === 'Uint8Array') {
            if (copy) {
                if (arg.slice) {
                    arg = arg.slice();
                } else {
                    arg = Array.prototype.slice.call(arg);
                }
            }
            return arg;
        }
        // It's an array; check it is a valid representation of a byte
        if (Array.isArray(arg)) {
            if (!checkInts(arg)) {
                throw new Error('Array contains invalid value: ' + arg);
            }
            return new Uint8Array(arg);
        }
        // Something else, but behaves like an array (maybe a Buffer? Arguments?)
        if (checkInt(arg.length) && checkInts(arg)) {
            return new Uint8Array(arg);
        }
        throw new Error('unsupported array-like object');
    }
    return {
        stringToBytes: stringToBytes,
        utf8BytesToUtf16String: utf8BytesToUtf16String,
        toBytes: toURIBytes,
        fromBytes: utf8ByteToUnicodeStr
    }
})()