#include "engine/experimental/ComponentVisitor.h"

#include "engine/experimental/Actor.h"
#include "engine/experimental/WorldComponent.h"

#include "pt/logging.h"

#include <assert.h>

//using namespace engine;
using namespace engine::experimental;



void ComponentVisitor::
AddWorldComponent()
{
    WorldComponent* wc = dynamic_cast< WorldComponent* >( mComponent );
    assert( wc );
    if( !wc ){
        pt::log::err << "ComponentVisitor: Invalid component conversion in Actor '" << mActor->GetName() << "'\n";
    }else{
        mActor->AddedWorldComponent( wc );
    }
}


void ComponentVisitor::
RemoveWorldComponent()
{
    WorldComponent* wc = dynamic_cast< WorldComponent* >( mComponent );
    assert( wc );
    if( !wc ){
        pt::log::err << "ComponentVisitor: Invalid component conversion in Actor '" << mActor->GetName() << "'\n";
    }else{
        mActor->RemovedWorldComponent( wc );
    }
}
