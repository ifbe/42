#import "Cocoa/Cocoa.h"
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "libuser.h"
void* supply_alloc();
void* supply_recycle(void*);




enum VertexAttributes {
	VertexAttributePosition = 0,
	VertexAttributeColor = 1,
};
enum BufferIndex  {
	MeshVertexBuffer = 0,
	FrameUniformBuffer = 1,
};
struct Vertex {
	float position[3];
	unsigned char color[4];
};
NSString* source =
@"#include <metal_matrix>\n"
"using namespace metal;\n"

"struct Uniform {\n"
"	metal::float4x4 projectionViewModel;\n"
"};\n"
"struct VertexInput {\n"
"	float3 where [[ attribute(0) ]];\n"
"	float4 color [[ attribute(1) ]];\n"
"};\n"
"struct VertOutFragIn {\n"
"	float4 where [[ position ]];\n"
"	float4 color;\n"
"};\n"

"vertex VertOutFragIn vert(\n"
"	VertexInput in [[ stage_in ]],\n"
"	constant Uniform& uni [[ buffer(1) ]] )\n"
"{\n"
"	VertOutFragIn out;\n"
"	out.where = uni.projectionViewModel * float4(in.where, 1.0);\n"
"	out.color = in.color / 255.0;\n"
"	return out;\n"
"}\n"
"fragment half4 frag(\n"
"	VertOutFragIn in [[stage_in]],\n"
"	texture2d<half> colorTexture [[ texture(0) ]])\n"
"{\n"
"	constexpr sampler sam(mag_filter::linear, min_filter::linear);\n"
"	return colorTexture.sample(sam, in.color.xy);\n"
"}\n";




_obj* thewnd = 0;
static void CreateApplicationMenus(void)
{
	id appName = [[NSProcessInfo processInfo] processName];
	id quitTitle = [@"Quit " stringByAppendingString:appName];
	id quitMenuItem = [[[NSMenuItem alloc]
		initWithTitle:quitTitle
		action:@selector(terminate:)
		keyEquivalent:@"q"] autorelease];

	id appMenu = [[NSMenu new] autorelease];
	[appMenu addItem:quitMenuItem];

	id appMenuItem = [[NSMenuItem new] autorelease];
	[appMenuItem setSubmenu:appMenu];

	id menubar = [[NSMenu new] autorelease];
	[menubar addItem:appMenuItem];
	[NSApp setMainMenu:menubar];
}




@interface MyApplicationDelegate : NSObject <NSApplicationDelegate> {
}
@end

@implementation MyApplicationDelegate : NSObject
- (id)init{
NSLog(@"init MyApplicationDelegate");
	self = [super init];
	if(nil != self){
	}
	return self;
}
- (void)windowDidResize:(NSNotification*)noti{
	NSLog(@"windowDidResize");
}
- (void)applicationWillFinishLaunching:(NSNotification*)noti{
	NSLog(@"applicationWillFinishLaunching");
	CreateApplicationMenus();
}
- (void)applicationDidFinishLaunching:(NSNotification*)noti{
	NSLog(@"applicationDidFinishLaunching");
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	[NSApp activateIgnoringOtherApps:YES];
}
- (void)applicationShouldTerminate{
	NSLog(@"applicationShouldTerminate");
}
- (void)applicationWillTerminate{
	NSLog(@"applicationWillTerminate");
}
- (void)applicationDidChangeScreenParameters{
	NSLog(@"applicationDidChangeScreenParameters");
}
- (void)dealloc{
	NSLog(@"free");
	[super dealloc];
}
@end




@interface MyWindowDelegate : NSObject <NSWindowDelegate> {
}
@end

@implementation MyWindowDelegate : NSObject
- (id)init{
NSLog(@"init MyWindowDelegate");
	self = [super init];
	if(nil != self){
	}
	return self;
}
- (void)windowWillBeginSheet:(NSNotification*)noti{
	NSLog(@"windowWillBeginSheet");
}
- (void)windowDidEndSheet:(NSNotification*)noti{
	NSLog(@"windowDidEndSheet");
}
- (void)windowDidResize:(NSNotification*)noti{
	NSLog(@"windowDidResize");
}
- (void)windowWillMiniaturize:(NSNotification*)noti{
	NSLog(@"windowWillMiniaturize");
}
- (void)windowDidMiniaturize:(NSNotification*)noti{
	NSLog(@"windowDidMiniaturize");
}
- (void)windowDidDeminiaturize:(NSNotification*)noti{
	NSLog(@"windowDidDeminiaturize");
}
- (void)windowWillEnterFullScreen:(NSNotification*)noti{
	NSLog(@"windowWillEnterFullScreen");
}
- (void)windowDidEnterFullScreen:(NSNotification*)noti{
	NSLog(@"windowDidEnterFullScreen");
}
- (void)windowWillExitFullScreen:(NSNotification*)noti{
	NSLog(@"windowWillExitFullScreen");
}
- (void)windowDidExitFullScreen:(NSNotification*)noti{
	NSLog(@"windowDidExitFullScreen");
}
- (void)windowWillClose:(NSNotification*)noti{
	NSLog(@"windowWillClose");
	eventwrite(0, 0, 0, 0);
}
-(void)mouseDown:(NSEvent *)event{
NSLog(@"mywindow.mouseDown");
}
-(void)mouseUp:(NSEvent *)event{
NSLog(@"mywindow.mouseUp");
}
-(void)keyDown:(NSEvent *)event{
NSLog(@"mywindow.keyDown");
}
-(void)keyUp:(NSEvent *)event{
NSLog(@"mywindow.keyUp");
}
@end




@interface MyView : MTKView
@end

@implementation MyView {
	//global
	id<MTLDevice> device;
	id<MTLCommandQueue> _commandQueue;
	id<MTLDepthStencilState> _depthState;

	//todo: bad design
	id <MTLCommandBuffer> commandBuffer;
/*
	//example
	id<MTLRenderPipelineState> _pipelineState;
	id<MTLLibrary> _shader;
	id<MTLTexture> _texture;
	id<MTLBuffer> _vertexBuffer;
	id<MTLBuffer> _uniformBuffer;*/
}
- (id)initWithFrame:(CGRect)inFrame {
NSLog(@"initWithFrame");
    device = MTLCreateSystemDefaultDevice();

    self = [super initWithFrame:inFrame device:device];
    if (self) {
		_commandQueue = [self.device newCommandQueue];
        [self setup];
    }
    return self;
}
- (void)upload_vertex {

}
- (void)setup {
NSLog(@"setup");
	// Set view settings.
	self.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
	self.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;

	// Create depth state.
	MTLDepthStencilDescriptor* depthDesc = [MTLDepthStencilDescriptor new];
	depthDesc.depthCompareFunction = MTLCompareFunctionLess;
	depthDesc.depthWriteEnabled = YES;
	_depthState = [self.device newDepthStencilStateWithDescriptor:depthDesc];

	//[self testprep];
}/*
- (void)testprep
{
	// Load shaders.
	NSError* error = nil;
	_shader = [self.device newLibraryWithSource:source options:nil error:&error];
	//_shader = [self.device newLibraryWithFile: @"metal.so" error:&error];
	if(!_shader) {
		NSLog(@"Failed to load library. error %@", error);
		exit(0);
	}
	id <MTLFunction> vertFunc = [_shader newFunctionWithName:@"vert"];
	id <MTLFunction> fragFunc = [_shader newFunctionWithName:@"frag"];

	// Create vertex descriptor.
	MTLVertexDescriptor* vertDesc = [MTLVertexDescriptor new];
	vertDesc.attributes[VertexAttributePosition].format = MTLVertexFormatFloat3;
	vertDesc.attributes[VertexAttributePosition].offset = 0;
	vertDesc.attributes[VertexAttributePosition].bufferIndex = MeshVertexBuffer;
	vertDesc.attributes[VertexAttributeColor].format = MTLVertexFormatUChar4;
	vertDesc.attributes[VertexAttributeColor].offset = 4*3;
	vertDesc.attributes[VertexAttributeColor].bufferIndex = MeshVertexBuffer;
	vertDesc.layouts[MeshVertexBuffer].stride = 4*4;
	vertDesc.layouts[MeshVertexBuffer].stepRate = 1;
	vertDesc.layouts[MeshVertexBuffer].stepFunction = MTLVertexStepFunctionPerVertex;

	// Create pipeline state.
	MTLRenderPipelineDescriptor* pipelineDesc = [MTLRenderPipelineDescriptor new];
	pipelineDesc.sampleCount = self.sampleCount;
	pipelineDesc.vertexFunction = vertFunc;
	pipelineDesc.fragmentFunction = fragFunc;
	pipelineDesc.vertexDescriptor = vertDesc;
	pipelineDesc.colorAttachments[0].pixelFormat = self.colorPixelFormat;
	pipelineDesc.depthAttachmentPixelFormat = self.depthStencilPixelFormat;
	pipelineDesc.stencilAttachmentPixelFormat = self.depthStencilPixelFormat;
	_pipelineState = [self.device newRenderPipelineStateWithDescriptor:pipelineDesc error:&error];
	if (!_pipelineState) {
		NSLog(@"Failed to create pipeline state, error %@", error);
		exit(0);
	}

	//texture
	MTLTextureDescriptor* textureDescriptor = [[MTLTextureDescriptor alloc] init];
	textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
	textureDescriptor.width = 1024;
	textureDescriptor.height = 1024;
	_texture = [self.device newTextureWithDescriptor:textureDescriptor];

	int x,y;
	unsigned int* image = malloc(4*1024*1024);
	for(y=0;y<1024;y++){
		for(x=0;x<1024;x++){
			if(((x&0xff) > 0x7f) && ((y&0xff) > 0x7f))image[y*1024+x] = 0xffffffff;
			else image[y*1024+x] = x | (y<<10);
		}
	}

	MTLRegion region = {
		{   0,    0, 0},	// MTLOrigin
		{1024, 1024, 1}		// MTLSize
	};
	[_texture replaceRegion:region
		mipmapLevel:0
		withBytes:image
		bytesPerRow:4*1024
	];

	//vertex
	struct Vertex verts[] = {
		{{-0.5,-0.5, 0}, {255, 0, 0, 255}},
		{{-0.5, 0.5, 0}, {0, 255, 0, 255}},
		{{ 0.5,-0.5, 0}, {0, 0, 255, 255}}
	};
	_vertexBuffer = [self.device
		newBufferWithBytes:verts
		length:sizeof(verts)
		options:MTLResourceStorageModeShared
	];
	_uniformBuffer = [self.device
		newBufferWithLength:sizeof(64)
		options:MTLResourceCPUCacheModeWriteCombined
	];
}
- (void)testshow:(id <MTLCommandBuffer>)cmdbuf
{
	//viewport
	MTLViewport vp = {
	.originX = 0.0,
	.originY = 0.0,
	.width = self.drawableSize.width,
	.height= self.drawableSize.height,
	.znear= 0.0,
	.zfar = 1.0
	};

	//
	float (*mat)[4] = (float (*)[4])[_uniformBuffer contents];
	mat[0][0] = 1.0;mat[0][1] = 0.0;mat[0][2] = 0.0;mat[0][3] = 0.0;
	mat[1][0] = 0.0;mat[1][1] = 1.0;mat[1][2] = 0.0;mat[1][3] = 0.0;
	mat[2][0] = 0.0;mat[2][1] = 0.0;mat[2][2] = 1.0;mat[2][3] = 0.0;
	mat[3][0] = 0.5;mat[3][1] = 0.0;mat[3][2] = 0.0;mat[3][3] = 1.0;

	// clear color
	MTLRenderPassDescriptor* passdesc = self.currentRenderPassDescriptor;
	passdesc.depthAttachment.loadAction = MTLLoadActionClear;
	passdesc.depthAttachment.clearDepth = 1.0;
	passdesc.colorAttachments[0].loadAction = MTLLoadActionLoad;
	//passdesc.colorAttachments[0].clearColor = MTLClearColorMake(0.5, 0.5, 0.5, 1.0);

	// Encode render command.
	id <MTLRenderCommandEncoder> encoder = [cmdbuf renderCommandEncoderWithDescriptor:passdesc];
	[encoder setViewport:vp];
	[encoder setDepthStencilState:_depthState];
	[encoder setRenderPipelineState:_pipelineState];
	[encoder setFragmentTexture:_texture atIndex:0];
	[encoder setVertexBuffer:_vertexBuffer offset:0 atIndex:MeshVertexBuffer];
	[encoder setVertexBuffer:_uniformBuffer offset:0 atIndex:FrameUniformBuffer];
	[encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
	[encoder endEncoding];
}*/
- (void)draw000:(id <MTLCommandBuffer>)cmdbuf
{
	// clear color
	MTLRenderPassDescriptor* passdesc = self.currentRenderPassDescriptor;
	passdesc.depthAttachment.loadAction = MTLLoadActionClear;
	passdesc.depthAttachment.clearDepth = 1.0;
	passdesc.colorAttachments[0].loadAction = MTLLoadActionClear;
	passdesc.colorAttachments[0].clearColor = MTLClearColorMake(0.1, 0.1, 0.1, 1.0);

	//clear color
	id <MTLRenderCommandEncoder> encoder = [cmdbuf renderCommandEncoderWithDescriptor:passdesc];
	[encoder endEncoding];
}
- (void)drawone:(id <MTLCommandBuffer>)cmdbuf
           data:(struct mt20data*)mt
           view:(MTLViewport)vp
{
//--------shader--------
	if(mt->dst.shader_deq != mt->src.shader_enq){
		if(mt->dst.shader){
			//free shader
		}
		if(mt->src.vs){
			NSError* error = nil;
			NSString* str = [NSString stringWithUTF8String:mt->src.vs];

			id<MTLLibrary> mtllib = [self.device newLibraryWithSource:str options:nil error:&error];
			if(!mtllib) {
				NSLog(@"Failed to load library. error %@", error);
				return;
			}
			id <MTLFunction> vertFunc = [mtllib newFunctionWithName:@"vert"];
			id <MTLFunction> fragFunc = [mtllib newFunctionWithName:@"frag"];

			// Create vertex descriptor.
			MTLVertexDescriptor* vertDesc = [MTLVertexDescriptor new];
			switch(mt->src.vtx[0].vbuf_fmt){
			case vbuffmt_333:
				vertDesc.attributes[0].format = MTLVertexFormatFloat3;
				vertDesc.attributes[0].offset = 0;
				vertDesc.attributes[0].bufferIndex = 0;
				vertDesc.attributes[1].format = MTLVertexFormatFloat3;
				vertDesc.attributes[1].offset = 4*3;
				vertDesc.attributes[1].bufferIndex = 0;
				vertDesc.attributes[2].format = MTLVertexFormatFloat3;
				vertDesc.attributes[2].offset = 4*6;
				vertDesc.attributes[2].bufferIndex = 0;
				vertDesc.layouts[0].stride = 4*9;
				vertDesc.layouts[0].stepRate = 1;
				vertDesc.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
				break;
			case vbuffmt_33:
				vertDesc.attributes[0].format = MTLVertexFormatFloat3;
				vertDesc.attributes[0].offset = 0;
				vertDesc.attributes[0].bufferIndex = 0;
				vertDesc.attributes[1].format = MTLVertexFormatFloat3;
				vertDesc.attributes[1].offset = 4*3;
				vertDesc.attributes[1].bufferIndex = 0;
				vertDesc.layouts[0].stride = 4*6;
				vertDesc.layouts[0].stepRate = 1;
				vertDesc.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
				break;
			}

			// Create pipeline state.
			MTLRenderPipelineDescriptor* pipelineDesc = [MTLRenderPipelineDescriptor new];
			pipelineDesc.sampleCount = self.sampleCount;
			pipelineDesc.vertexFunction = vertFunc;
			pipelineDesc.fragmentFunction = fragFunc;
			pipelineDesc.vertexDescriptor = vertDesc;
			pipelineDesc.colorAttachments[0].pixelFormat = self.colorPixelFormat;
			pipelineDesc.depthAttachmentPixelFormat = self.depthStencilPixelFormat;
			pipelineDesc.stencilAttachmentPixelFormat = self.depthStencilPixelFormat;

			id<MTLRenderPipelineState> pipeline = [self.device newRenderPipelineStateWithDescriptor:pipelineDesc error:&error];
			if (!pipeline) {
				NSLog(@"Failed to create pipeline state, error %@", error);
				exit(0);
			}

			mt->dst.shader = mtllib;
			mt->dst.vert = vertFunc;
			mt->dst.frag = fragFunc;
			mt->dst.pipeline = pipeline;
			NSLog(@"upload shader");
		}
		mt->dst.shader_deq = mt->src.shader_enq;
	}


//--------vertex--------
	struct vertex* vtx = &mt->src.vtx[0];
	if(0 == vtx->vbuf_h)return;
	if(mt->dst.vbo_deq != mt->src.vbuf_enq){
		if(vtx->vbuf){
			if(0 == mt->dst.vbuf){
				mt->dst.vbuf = [self.device
					newBufferWithBytesNoCopy:vtx->vbuf
					length:vtx->vbuf_len
					options:MTLResourceStorageModeShared
					deallocator:nil
				];
				NSLog(@"upload vertex:%x,%x,%x", vtx->vbuf_w, vtx->vbuf_h, vtx->vbuf_len);
			}
			else{

			}
		}
		mt->dst.vbo_deq = mt->src.vbuf_enq;
	}

	if(mt->dst.ibo_deq != mt->src.ibuf_enq){
		if(vtx->ibuf){
			if(0 == mt->dst.ibuf){
				mt->dst.ibuf = [self.device
					newBufferWithBytesNoCopy:vtx->ibuf
					length:vtx->ibuf_len
					options:MTLResourceStorageModeShared
					deallocator:nil
				];
				NSLog(@"upload indice:%x,%x,%x", vtx->ibuf_w, vtx->ibuf_h, vtx->ibuf_len);
			}
			else{

			}
		}
		mt->dst.ibo_deq = mt->src.ibuf_enq;
	}


//--------uniform--------
	if(0 == mt->dst.uniform){
		mt->dst.uniform = [self.device
			newBufferWithLength:sizeof(64)
			options:MTLResourceCPUCacheModeWriteCombined
		];
	}
	id<MTLBuffer> uniform = mt->dst.uniform;

	struct mt20world* world = &thewnd->mt20list.world[0];
	float (*cam)[4] = world->camera[0]->src.uni[0].buf;

	int x,y;
	float (*tmp)[4] = (float (*)[4])[uniform contents];
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)tmp[y][x] = cam[y][x];
	}


//--------drawcall--------
	// clear color
	MTLRenderPassDescriptor* passdesc = self.currentRenderPassDescriptor;
	passdesc.depthAttachment.loadAction = MTLLoadActionClear;
	passdesc.depthAttachment.clearDepth = 1.0;
	passdesc.colorAttachments[0].loadAction = MTLLoadActionLoad;
	//passdesc.colorAttachments[0].clearColor = MTLClearColorMake(0.5, 0.5, 0.5, 1.0);

	// Encode render command.
	id <MTLRenderCommandEncoder> encoder = [cmdbuf renderCommandEncoderWithDescriptor:passdesc];
	[encoder setViewport:vp];
	[encoder setDepthStencilState:_depthState];
	[encoder setRenderPipelineState:mt->dst.pipeline];
	[encoder setVertexBuffer:mt->dst.vbuf offset:0 atIndex:0];
	[encoder setVertexBuffer:mt->dst.uniform offset:0 atIndex:1];
	switch(mt->src.vtx[0].geometry){
	case 2:
		[encoder drawIndexedPrimitives:MTLPrimitiveTypeLine
			indexCount:vtx->ibuf_h*2
			indexType:MTLIndexTypeUInt16
			indexBuffer:mt->dst.ibuf
			indexBufferOffset:0
		];
		break;
	case 3:
		[encoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle 
			indexCount:vtx->ibuf_h*3
			indexType:MTLIndexTypeUInt16
			indexBuffer:mt->dst.ibuf
			indexBufferOffset:0
		];
		break;
	}

	[encoder endEncoding];
}
- (void)draweach:(struct fstyle*)area
{
	//viewport: lefttop = (0,0), rightbot = (1.0,1.0)
	float x0 = self.drawableSize.width * area->vc[0];
	float y0 = self.drawableSize.height* (1.0 - area->vc[1] - area->vq[1]);
	float ww = self.drawableSize.width * area->vq[0];
	float hh = self.drawableSize.height* area->vq[1];
	MTLViewport vp = {
	.originX = x0,
	.originY = y0,
	.width = ww,
	.height= hh,
	.znear= 0.0,
	.zfar = 1.0
	};

	//foreach mesh
	int j;
	struct mt20data* mt;
	for(j=0;j<64;j++){
		mt = thewnd->mt20list.world[0].solid[j];
		if(0 == mt)continue;

		//NSLog(@"%d, %p", j, mt);
		[self drawone:commandBuffer data:mt view:vp];
	}
}
- (void)drawprep
{
	//create commandbuffer.
	commandBuffer = [_commandQueue commandBuffer];

	//clear color
	[self draw000:commandBuffer];
}
- (void)drawdone
{
	//debug
	//[self testshow:commandBuffer];

	//call gpu
	[commandBuffer presentDrawable:self.currentDrawable];
	[commandBuffer commit];
}
- (void)drawRect:(CGRect)rect
{
//NSLog(@"drawRect");
	//[self drawall];
	[super drawRect:rect];
}
-(void)mouseDown:(NSEvent *)event{
NSLog(@"mouseDown");
NSPoint location_in_window = [event locationInWindow];
NSLog(@"window:%lf,%lf", location_in_window.x, location_in_window.y);
NSPoint location_in_myview = [self convertPoint:location_in_window fromView:nil];
NSLog(@"myview:%lf,%lf", location_in_myview.x, location_in_myview.y);
}
-(void)mouseUp:(NSEvent *)event{
NSLog(@"mouseUp");
}
-(void)mouseMoved:(NSEvent *)event{
NSLog(@"mouseMoved");
}
-(void)mouseDragged:(NSEvent *)event{
NSLog(@"mouseDragged");
}
@end




int fullwindow_taking(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	MyView* view = thewnd->mt20list.view;
	[view drawprep];

	//take
	struct relation* rel = (struct relation*)wnd->orel0;
	while(1){
		if(0 == rel)break;

		struct fstyle* area = (struct fstyle*)rel->psrcfoot;
		if(area){
			//get vertex
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			//stack[sp+0].type = rel->srctype;
			stack[sp+0].foottype = rel->srcfoottype;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			//stack[sp+1].type = rel->dsttype;
			stack[sp+1].foottype = rel->dstfoottype;
			entity_takeby((_obj*)rel->pdstchip, rel->pdstfoot, stack,sp+2, 0,'v', 0, 0);
		}

		[view draweach:area];

		//next
		rel = (struct relation*)samesrcnextdst(rel);
	}

	[view drawdone];
	return 0;
}
int fullwindow_giving(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	fullwindow_taking(wnd,foot, stack,sp, arg,key, buf,len);

	while(1){
		NSEvent *event = [NSApp
			nextEventMatchingMask:NSEventMaskAny
			untilDate:[NSDate distantPast]
			inMode:NSDefaultRunLoopMode
			dequeue:YES
		];
		if(nil == event)break;

		switch ([event type]) {
		case NSEventTypeLeftMouseDown:
		case NSEventTypeOtherMouseDown:
		case NSEventTypeRightMouseDown:
		case NSEventTypeLeftMouseUp:
		case NSEventTypeOtherMouseUp:
		case NSEventTypeRightMouseUp:
		case NSEventTypeLeftMouseDragged:
		case NSEventTypeRightMouseDragged:
		case NSEventTypeOtherMouseDragged: /* usually middle mouse dragged */
		case NSEventTypeMouseMoved:
		case NSEventTypeScrollWheel:
		case NSEventTypeKeyDown:
		case NSEventTypeKeyUp:
		case NSEventTypeFlagsChanged:
		default:break;
		}
		/* Pass through to NSApp to make sure everything stays in sync */
		[NSApp sendEvent:event];
	}
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
void window_attach()
{
}
void window_detach()
{
}




void window_read()
{
}
void window_write()
{
}
void window_delete(_obj* w)
{
}
void window_create(_obj* wnd)
{
	//wnd->tier
	//wnd->type
	wnd->hfmt = _mt20list_;
	wnd->vfmt = _mt20list_;

	wnd->whdf.width = 1024;
	wnd->whdf.height = 768;
	wnd->whdf.fbwidth = 1024;
	wnd->whdf.fbheight = 768;

	wnd->mt20list.world[0].camera = (struct mt20data**)memoryalloc(0x10000, 0);
	wnd->mt20list.world[0].light  = (struct mt20data**)memoryalloc(0x10000, 0);
	wnd->mt20list.world[0].solid  = (struct mt20data**)memoryalloc(0x10000, 0);
	wnd->mt20list.world[0].opaque = (struct mt20data**)memoryalloc(0x10000, 0);
	thewnd = wnd;


	//window
	NSRect windowRect = NSMakeRect(0, 0, 1024, 768);
	NSUInteger windowStyle =
		NSWindowStyleMaskTitled |
		NSWindowStyleMaskClosable |
		NSWindowStyleMaskResizable |
		NSWindowStyleMaskMiniaturizable;
	id window = [[[NSWindow alloc]
		initWithContentRect:windowRect
		styleMask:windowStyle
		backing:NSBackingStoreBuffered
		defer:NO]
	autorelease];
	[window makeKeyAndOrderFront:nil];

	id appName = [[NSProcessInfo processInfo] processName];
	[window setTitle:appName];

	MyWindowDelegate* windowdelegate = [[[MyWindowDelegate alloc] init] autorelease];
	[window setDelegate:windowdelegate];


	//view
	MyView* myview = [[[MyView alloc] initWithFrame:windowRect] autorelease];
	[window setContentView:myview];

	wnd->mt20list.delegate = windowdelegate;
	wnd->mt20list.view = myview;
}




void* window_alloc()
{
	return supply_alloc();
}




void initwindow()
{
	//app
	[NSApplication sharedApplication];

	MyApplicationDelegate* applicationdelegate = [[[MyApplicationDelegate alloc] init] autorelease];
	[NSApp setDelegate:applicationdelegate];

	[NSApp finishLaunching];
}
void freewindow()
{
}
