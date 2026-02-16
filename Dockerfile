FROM learnwithexamples/systemc

WORKDIR /workspace

# Set SystemC environment variables
ENV SYSTEMC=/usr/local/systemc-2.3.3
ENV LD_LIBRARY_PATH=/usr/local/systemc-2.3.3/lib-linux64

# Copy your source files
COPY . .

# Default command - can be overridden
CMD ["bash"]
