# CPP-Socket-Curl
A CLI tool that makes a HTTP request to any page


# Systems Assignment

## What is it?

In this assignment you'll write a program that makes a request to the endpoints you created in the General Assignment.  This is a systems assignment so we want to see that you're able to use sockets directly rather than using a library that handles the HTTP request.

## Requirements

### 1. Use one of the specified languages

Choose from among C/C++/Go/Rust. If you aren't familiar with these languages, you're not alone! Many engineers join Cloudflare without
specific language experience. See the Useful Links section for some quickstart guides.

### 2. Use an off the shelf build tool

Choose something to build your assignment that works with the language you chose (Cargo, Make, CMake etc.).  Include instructions in your readme on how to build and run your program.  Don't check-in binaries, we won't run a pre-compiled binary.

### 3. Do **NOT** use a library to handle the HTTP request

We want to see how familiar you are with systems work.  Although we would normally recommend using a library to handle HTTP requests, for this assignment we want to see how you handle it yourself.

### 4. Create a CLI tool that makes a request to your links page

Your CLI tool should take an argument that is a full URL (--url).  The tool will make an HTTP request to the URL and print the response directly to the console.  Test the CLI tool by specifying the /links URL in your General Assignment and make sure it prints the entire json document with all your links.

Your CLI tool should also allow a --help parameter that describes how to use it.

Feel free to use a library to handle command line argument parsing (getopt etc.).

### 5. Measure how fast it is

Next, add logic to your tool to profile your page.  Add a new argument --profile that takes a positive integer.  Your tool should make that number of requests to your site.  Time the requests and print:

* The number of requests
* The fastest time
* The slowest time
* The mean & median times
* The percentage of requests that succeeded
* Any error codes returned that weren't a success
* The size in bytes of the smallest response
* The size in bytes of the largest response

Include a screenshot of your tool run against your site and another webpage.

Test your tool against your site and some other websites.  Let us know what you find in your readme.  Include outputs for popular sites and your own.  How do we compare?

