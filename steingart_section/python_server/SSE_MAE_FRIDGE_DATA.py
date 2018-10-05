##pithytimeout=0 
from commands import getoutput as go
print go("pip install sseclient")
from sseclient import SSEClient
import json
import time
from pymongo import MongoClient as mc

mip = "" ##Your MongoDB Address here

client = mc(mip,27017)
#print client
cores = {}

ac = "" #->Get you token for this from Particle Build
get_event = "MAE_519_LAB_1" #this is the event we declared in "particle.publish"

while True:
    try:
        messages = SSEClient(
            'https://api.particle.io/v1/events/%s?access_token=%s' %(get_event,ac) 
            )
        #connecto the streaming https interface from particle, get the event for our token
        for msg in messages: #for each message that comes in
            try:
                foo = msg.data.replace("nan","NaN") #format NaN correctly for python
                total = json.loads(foo) #load the dataset as a JSON string
                data = json.loads(total['data']) #load the data payload as a JSON string
                data['time'] = time.time() #add a timestamp
                data['coreid'] = total['coreid'] #add the particle name
                try: #try to make an event index for the structure
                    data['tid'] = client[get_event][data['coreid']].find().count()+1
                    client[get_event][ data['coreid'] ].insert(data)
                except Exception as E:
                    print "try to insert:",E

                #this next block makes a status stable of the last reading of the node
                data['_id'] = data['coreid']
                _id = data['coreid'] 
                client[get_event]['status'].update({"_id":_id},{'$set':data},upsert=True)

            except Exception as E:
                print E,foo
                data = {'error':str(E),'time':time.time()}
                client[get_event+'_error'].errorlog.insert(data)

    except Exception as E:
        print "Data error: try",E 

   #if we have an error, we wait five seconds and then restablish the steaming https connection.     
   time.sleep(5)




