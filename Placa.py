import cv2
import imutils 
import pytesseract
import requests

pytesseract.pytesseract.tesseract_cmd -r"C:\ProgramFiles\Tesseract-OCR\tesseract.exe"
image = cv2.imread('car0.jpg')
image = imutils.resize(image , width = 500)

cv2.imshow("Original Image",image)
cv2.waitkey(0)

gray = cv2.cvtColor(image , cv2.COLOR_BRG2GRAY)
cv2.imshow("Gray Scale Image",gray)
cv2.waitkey(0)

gray = cv2.bilateralFilter(gray ,11 , 17 , 17)
cv2.imshow("Smoother Image",gray)
cv2.waitkey(0)

edged = cv2.Canny(gray ,170 ,200)
cv2.imshow("Canny edge",edged)
cv2.waitkey(0)

cnts , new = cv2.findContours(edged.copy() ,cv2RETR_LIST , cv2.CHAIN_APPROX_SIMPLE)

image1 = image.copy()
cv2.drawContours(image1 , cnts , -1 , (0,255,0)3)
cv2.imshow("Canny after contouring",image1)
cv2.waitkey(0)

cnts = sorted(cnts , key = cv2.contourArea , reverse = true)[:30]
NumberPlateCount = None

Image2 = image.copy()
cv2.drawContours(image2 , cnts , -1 , (0,255,0)3)
cv2.imshow("Top 30 Countours",image2)
cv2.waitkey(0)

count = 0
nane = 1

for i in cnts:
    perimeter = cv2.arcLength(i, True)
    approx = cv2.approxPolyDP(i , 0.002*perimeter ,True)
    if(len(approx) == 4):
        NumberPlateCount = approx
        x , y , w , h = cv2.boundingRect(i)
        crp_img = image[y:y+h , x:x+w]
        
        cv2.imwrite(str(name)+ '.png',crp_img)
        name += 1
        
        break 
        
cv2.drawContours(image,[NumberPlateCount], -1 , (0,255,0)3)
cv2.imshow("Final Image",image)
cv2.waitkey(0)
    
crop_img_loc = '1.png'
cv2.imshow("Cropped Image ",cv2.imread(crop_img_loc))    
cv2.waitkey(0)

text = pytesseract.image_to_string(crop_img_loc,Lang='eng')
print("Number is : ", text)
cv2.waitkey(0)

placa = text
if placa = = 'AUW-9454':
	ploads1 = {'password':'senha','function':'0'}
	r1 = requests.get('http://192.168.0.110/abertura',params=ploads1)
	ploads = {'password':'senha','function':'1'}
	r = requests.get('http://192.168.0.110/abertura',params=ploads)
    print('Cancela liberada.')
else:
    print('Liberação cancelada.')


